# Three currency pairs, one coefficient

This small C++ and Python experiment aligns EUR/CAD, CAD/CHF and EUR/CHF price series and plots their triangular conversion coefficient. For an initial volume of one, the example computes `EURCAD × CADCHF ÷ EURCHF`.

[Read the original exercise in English](EXERCISE.md).

## Reproduce the figures

| Stage | Input | Output |
| --- | --- | --- |
| [C++ calculation](part_1.cpp) | The three price CSVs, the pair to invert and an initial volume | `out.csv` |
| [Python analysis](part_2_3.py) | Price and coefficient series | Plotly figures showing prices, the filtered coefficient, its derivative and extrema |

From the repository root, using a C++17 compiler and Python:

```sh
c++ -std=c++17 part_1.cpp -o pairs
./pairs . EURCHF 1
python3 -m pip install -r requirements.txt
python3 part_2_3.py
```

To explore the saved figures, open [part2.html](part2.html) and [part3.html](part3.html) from your local checkout.
