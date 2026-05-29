# Plazza

Creating pizzas with processes (using IPC) & threads

## Build

```sh
make -j`nproc`
```

## Usage

```sh
./plazza 2 5 2000
```

First argument corresponds to the multiplier (can be float) used for cooking the pizzas.

Second argument is the number of cooks per kitchen.

Third argument is the time it takes to refill a kitchen.
