# Comp-3005-Final-Project-

## Overview
This is a C++ console application for managing a fitness center.  

It connects to a PostgreSQL database to manage members, trainers and admins. Members can sign up, scheledule personal training sessions, and update/add health metrics and set fitness goals. Trainers can add thier availability, view thier schedule and view a member's progress. Admins can view a room's schedule, book a room for a PT session, add equipment, log equipment maintenance, and they can create a class, update the capacity and cancel a class.

The app uses **libpq** (PostgreSQL C API) for database interactions and a **Makefile** for building the project.

---

## Features
- Manage **Members**, **Trainers**, and **Admins**
- Track **Personal Training Sessions** and **Group Classes**
- Record **Health Metrics** (weight, height, heart rate)
- Set **Fitness Goals** for members
- Manage **Trainer Availability**
- Track **Equipment** and **Maintenance Logs**
- Admin functionalities:
  - View and book rooms
  - Add equipment and log maintenance
  - Create, update, and cancel classes

---

## Requirements
- **Windows 10/11**
- **MSYS2** with **UCRT64**
- **g++** compiler
- **make**
- **PostgreSQL 18**
- **C++17** or higher

---

## Setup Instructions

### 1. Install MSYS2 and PostgreSQL

You should already have PostgreSQL but if not,

- Download and install [MSYS2](https://www.msys2.org/)
- Install PostgreSQL 18 and note your `postgres` user password

### 2. Install C++ toolchain in MSYS2 UCRT64
Open **MSYS2 UCRT64** and run:

```bash
pacman -S --needed mingw-w64-ucrt-x86_64-toolchain make
````

### 3. Set up the PostgreSQL database

1. Open fitness_setup.sql 

```sql
psql -U postgres -d fitness -f fitness_setup.sql
```

I had a corrupt file that prevent me from connecting to PG Admin 4, so I'm putting this here just in case

2. Open `psql`:

```bash
psql -U postgres
```

3. Make sure you can connect to the `fitness` database:

```sql
\c fitness
```

---

### 4. Build the application

1. Open **MSYS2 UCRT64** and navigate to the project folder:

```bash
cd "/c/Users/path/to/folder/location"
```

2. Build with Make:

```bash
make clean
make
```

This removes any previous object files (there shouldn't be any but just in case) and compiles the source files and links them with **libpq** to create `fitness_app.exe`.

---

### 5. Run the application

```bash
./fitness_app.exe
```

Follow the console menu to:

* Eexplore Member Functions
* Explore Trainer Functions
* Explore Admin Functions

---

## File Structure

```
Final Project/
├── main.cpp
├── admin.cpp
├── admin.hpp
├── member.cpp
├── member.hpp
├── trainer.cpp
├── trainer.hpp
├── db.hpp
├── Makefile
└── README.md
```

* `db.hpp` – Handles PostgreSQL connection and query execution
* `admin.hpp` / `admin.cpp` – Admin functionalities
* `member.hpp` / `member.cpp` – Member functionalities
* `trainer.hpp` / `trainer.cpp` – Trainer functionalities
* `Makefile` – Build instructions
* `main.cpp` – Main program entry point

---

## Database Connection

Update your connection string in the main.cpp file code:

```cpp
Database db("host=localhost dbname=fitness user=postgres password=password");
```

Replace `password` with the password you set for `postgres`.

---

## Notes

* Make sure PostgreSQL is running before launching the app.
* The app must connect to the `fitness` database.
* Tables must be created before running the app, otherwise SQL errors will occur.

---

## Author

Ana Vanesa De Leon Bonilla
101279256
COMP 3005 - Final Project

