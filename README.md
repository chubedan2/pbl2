# Restaurant Management MVP

## Documentation

Project documentation is organized under [`docs/`](docs/README.md), including
requirements, architecture and data design, diagrams, file persistence details,
and verification notes. The original Vietnamese SRS v1.9 remains available at
[`SRS_nha_hang_services_repositories_v1.9.md`](SRS_nha_hang_services_repositories_v1.9.md).

## Requirements

- GNU Make
- A C++17 compiler (for example, `g++`)
- Qt 6 Widgets development files
- `pkg-config`

On Ubuntu, install the GUI build dependencies with:

```sh
sudo apt install build-essential pkg-config qt6-base-dev
```

## Build and run

From the project root:

```sh
make
make run
```

The executable is written to `build/restaurant_app`. Repository data is read
from and written to the `data/` directory relative to the current working
directory, so run the executable from the project root.

Run core model/service/repository tests without launching the GUI:

```sh
make test
```

To remove generated build files:

```sh
make clean
```

The application uses Qt Widgets and provides screens for tables and dining
sessions, menu items, orders, bills, and payments. Repository data is stored in
`data/` relative to the project root.
