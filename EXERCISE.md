# Currency triangle exercise

The input consists of three price files covering the same period: EUR/CAD, CAD/CHF and EUR/CHF. The program should also handle other combinations of two or more input files.

## 1. Compute the coefficient

Use a C-family language such as C, C++, C# or MQL. Start with one euro and multiply the prices in sequence, taking the reciprocal of EUR/CHF. Call the resulting coefficient *k*. Save each coefficient in the same format as the input price series.

## 2. Plot the prices and coefficient

Use Python and Plotly to display the three price series and *k*, reading the coefficient from the file produced in part 1. Put all four charts on one screen and save them together in one HTML file.

## 3. Find the extrema

Filter *k* using a method of your choice and calculate the derivative. Mark the extrema of *k* with vertical lines. Display the coefficient, derivative and marked extrema with Plotly and save the figure as HTML.

The supplied example visualization uses a different group of three instruments.
