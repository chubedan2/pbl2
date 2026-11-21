#pragma once

#include "../Structure/DynamicArray.h"

#include <filesystem>
#include <fstream>
#include <functional>
#include <stdexcept>
#include <string>
#include <utility>

namespace restaurant {

template <typename T>
class FileRepository {
public:
    typedef std::function<std::string(const T&)> IdReader;
    typedef std::function<void(std::ostream&, const T&)> Writer;
    typedef std::function<T(std::istream&)> Reader;

    FileRepository(const std::string& path,
                   IdReader idReader,
                   Writer writer,
                   Reader reader)
        : path_(path),
          idReader_(std::move(idReader)),
          writer_(std::move(writer)),
          reader_(std::move(reader)),
          items_() {
        if (path_.empty() || !idReader_ || !writer_ || !reader_) {
            throw std::invalid_argument("File repository configuration is invalid");
        }
        load();
    }

    const DynamicArray<T>& getAll() const noexcept {
        return items_;
    }

    const T* findById(const std::string& id) const {
        const std::size_t index = findIndex(id);
        return index == DynamicArray<T>::npos ? nullptr : &items_[index];
    }

    T* findById(const std::string& id) {
        const std::size_t index = findIndex(id);
        return index == DynamicArray<T>::npos ? nullptr : &items_[index];
    }

    bool add(const T& item) {
        const std::string id = idReader_(item);
        if (findIndex(id) != DynamicArray<T>::npos) {
            return false;
        }

        DynamicArray<T> next(items_);
        next.pushBack(item);
        persist(next);
        items_.swap(next);
        return true;
    }

    bool update(const T& item) {
        const std::size_t index = findIndex(idReader_(item));
        if (index == DynamicArray<T>::npos) {
            return false;
        }

        DynamicArray<T> next(items_);
        next[index] = item;
        persist(next);
        items_.swap(next);
        return true;
    }

    bool erase(const std::string& id) {
        const std::size_t index = findIndex(id);
        if (index == DynamicArray<T>::npos) {
            return false;
        }

        DynamicArray<T> next(items_);
        next.removeAt(index);
        persist(next);
        items_.swap(next);
        return true;
    }

private:
    std::size_t findIndex(const std::string& id) const {
        for (std::size_t i = 0; i < items_.size(); ++i) {
            if (idReader_(items_[i]) == id) {
                return i;
            }
        }
        return DynamicArray<T>::npos;
    }

    void load() {
        std::ifstream input(path_.c_str());
        if (!input.is_open()) {
            if (!std::filesystem::exists(path_)) {
                return;
            }
            throw std::runtime_error("Could not open repository file: " + path_);
        }

        std::size_t count = 0;
        if (!(input >> count)) {
            throw std::runtime_error("Invalid repository file header: " + path_);
        }

        DynamicArray<T> loaded;
        for (std::size_t i = 0; i < count; ++i) {
            T item = reader_(input);
            if (findIndexIn(loaded, idReader_(item)) != DynamicArray<T>::npos) {
                throw std::runtime_error("Duplicate ID in repository file: " + path_);
            }
            loaded.pushBack(std::move(item));
        }

        input >> std::ws;
        if (!input.eof()) {
            throw std::runtime_error("Unexpected trailing data in repository file: " + path_);
        }
        items_.swap(loaded);
    }

    void persist(const DynamicArray<T>& items) const {
        const std::filesystem::path target(path_);
        const std::filesystem::path parent = target.parent_path();
        if (!parent.empty()) {
            std::filesystem::create_directories(parent);
        }

        const std::string temporaryPath = path_ + ".tmp";
        {
            std::ofstream output(temporaryPath.c_str(), std::ios::trunc);
            if (!output.is_open()) {
                throw std::runtime_error("Could not write repository file: " + temporaryPath);
            }
            output << items.size() << '\n';
            for (std::size_t i = 0; i < items.size(); ++i) {
                writer_(output, items[i]);
                output << '\n';
                if (!output) {
                    throw std::runtime_error("Failed while writing repository file: " + temporaryPath);
                }
            }
            output.flush();
            if (!output) {
                throw std::runtime_error("Failed to flush repository file: " + temporaryPath);
            }
        }

        std::error_code error;
        std::filesystem::rename(temporaryPath, target, error);
        if (error) {
            std::filesystem::remove(temporaryPath);
            throw std::runtime_error("Could not replace repository file: " + path_ +
                                     " (" + error.message() + ")");
        }
    }

    static std::size_t findIndexIn(const DynamicArray<T>& items,
                                   const std::string& id,
                                   const IdReader& idReader) {
        for (std::size_t i = 0; i < items.size(); ++i) {
            if (idReader(items[i]) == id) {
                return i;
            }
        }
        return DynamicArray<T>::npos;
    }

    std::size_t findIndexIn(const DynamicArray<T>& items,
                            const std::string& id) const {
        return findIndexIn(items, id, idReader_);
    }

    std::string path_;
    IdReader idReader_;
    Writer writer_;
    Reader reader_;
    DynamicArray<T> items_;
};

}  // namespace restaurant
