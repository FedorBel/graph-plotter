# Graph Plotter
This application is for ploting data in real-time. Now it runs only from *Qt Creator*.

<img src="https://github.com/FedorBel/qt-serial-plotter/blob/master/docs/img/plotter-demo.PNG" width="659" height="522"/>

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes.

### Dependencies

| Name | Version | 
|----------------|:---------:|
| Build system | QMake version 3.1 |
| Qt | 5.14.0 | 
| qcustomplot | 2.0.1 | 

### Installing

Clone the repo

```
$ git clone https://github.com/FedorBel/qt-serial-plotter.git
```

### Usage

To run the program, open [qt-serial-plotter.pro](qt-serial-plotter.pro) with *Qt Creator* and click *Run*.

From microcontroller send this string (without '[]' ):

```
$[data]\r\n
```

Example program with [Arduino UNO](examples/arduino_example.cpp):

## Built With

* [Qt](https://www.qt.io/) - Free and open-source widget toolkit for creating graphical user interfaces
* [QCustomPlot](https://www.qcustomplot.com/) -  Widget for plotting and data visualization

## Versioning

We use [SemVer](http://semver.org/) for versioning. For the versions available, see the [tags on this repository](https://github.com/FedorBel/qt-serial-plotter/tags). 

## Authors
* **Fedor Belyakov** - *Initial work* - [FedorBel](https://github.com/FedorBel)

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details
