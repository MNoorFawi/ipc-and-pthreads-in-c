# Interprocess Communication & Parallelism in C
Concurrency and interprocess communication through named pipes and shared memory along with parallelism through posix threads to get weather data from an API

3 process will communicate with each other using different ways to view weather data obtained from https://openweathermap.org/api

1) **reader.c**: it reads the input file and puts the city names into the **named pipe** for the worker process
2) **worker.c**: it reads data from the named pipe, **parallelize** the api requests using **pthread**s and writes the results into the **shared memory object** for the viewer process
3) **viewer.c**: it reads the results from the shared memory object, writes it into a file and filter it then view it.

### Compile & Run
```bash
gcc reader.c -o read
gcc worker.c -o work -lpthread
gcc viewer.c -o view
```

In three different terminal, run each one:
```bash
./read cities.txt
```

```bash
./work
```

```bash
$ ./view

Cities with clear sky or clouds and degree <= 25:

| city      | temperature | description      |
| --------- | ----------- | ---------------- |
| Amsterdam |        4.85 | few clouds       |
| Berlin    |       -1.45 | broken clouds    |
| Havana    |       23.00 | scattered clouds |
| Cape Town |       23.17 | clear sky        |
| Sydney    |       18.25 | clear sky        |
| New York  |        6.03 | broken clouds    |
| Tokyo     |        3.10 | few clouds       |
| Madrid    |        4.43 | clear sky        |
| Paris     |        4.25 | few clouds       |
| Rome      |        8.05 | overcast clouds  |
| Dubai     |       21.23 | clear sky        |
| Cairo     |       16.56 | broken clouds    |

```
