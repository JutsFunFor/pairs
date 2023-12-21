### Clone the Project

To get started, clone the repository using the following command:

```git clone https://github.com/JutsFunFor/static_arbitrage.git```

```bash
cd ./static_arbitrage
pip install -r requirements.txt
```


## Part 1
Compile part_1.cpp file according your compiler

Usage for compiled file:

`part_1.exe <directory_path> <target_currency_pair> <volume>`

`directory_path` - path to directory with .csv files (for example `EURCAD.csv`)

`target_currency_pair` - currency pair that should be reversed (if specified `EURCHF` than `price` values of `EURCHF.csv` will become `1/price` in further calculus)

`volume` - initial volume

### Example:


```./part_1.exe ./ EURCHF 1```

It will generate `out.csv` file with coefficents. From previous example: coeff equals multiplication of prices EURCAD* CADCHF* 1/EURCHF * volume

## Part 2-3
Run script or check for .html files that previously were generated from this script in project folder

`python part_2_3.py`
