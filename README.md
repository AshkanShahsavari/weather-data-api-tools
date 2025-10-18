# Weather Data API Tools

## Description
A clean and modular C++ toolkit for querying, processing, and storing weather and environmental data from REST APIs.  
It includes HTTP communication, configuration management, structured logging, token handling, and optional InfluxDB persistence.

## Build Instructions
```bash
mkdir build && cd build
cmake ..
make -j
./bin/weather_data_api_tools
```

## Generate Documentation
To build Doxygen documentation, simply run:
```bash
cmake -DDOXYGEN_DOC=ON ..
```
The generated files will be found in the `docs/` directory.

## License
MIT License
