import numpy as np
import pandas as pd
import plotly.graph_objects as go
import plotly.io as pio
import os

def load_data(dir_path):
    all_data = {}
    for filename in os.listdir(dir_path):
        if filename.lower().endswith(".csv"):
            data = pd.read_csv(os.path.join(dir_path, filename), header=None, sep=';')
            data.columns = ["timestamp", "price"]
            all_data[filename[:-4]] = data
    return all_data

def part2(path):
    all_data = load_data(path)
    traces = []
    for key in all_data.keys():
        traces.append(go.Scatter(x=pd.to_datetime(all_data[f'{key}']['timestamp'], unit='s'), y=all_data[f'{key}']['price'], name=key))
    layout = go.Layout(title='Forex Prices and Coefficient k',
                       xaxis=dict(title='Time'),
                       yaxis=dict(title='Price / Coefficient'))
    fig = go.Figure(data=traces, layout=layout)
    fig.show()
    pio.write_html(fig, 'part2.html')


def filter_data(data):
    return np.convolve(data, np.ones(5)/5, mode='valid')

def derivative(data):
    return np.gradient(data)

def find_extremes(deriv):
    extremes_indices = np.where((deriv[:-2] > deriv[1:-1]) &
                                 (deriv[1:-1] < deriv[2:]))[0] + 1
    return extremes_indices

def part3(path):
    all_data = load_data(path)
    k_data = all_data['out']['price']
    filtered_data = filter_data(k_data)
    deriv = derivative(filtered_data)
    extreme_indices = find_extremes(deriv)

    trace_k = go.Scatter(y=k_data, mode='lines', name='k')
    trace_filtered_k = go.Scatter(y=filtered_data, mode='lines', name='Filtered k')
    trace_derivative = go.Scatter(y=deriv, mode='lines', name='Derivative of k')
    trace_extremes = go.Scatter(x=extreme_indices, y=[deriv[i] for i in extreme_indices],
                                mode='markers', name='Extremes', marker=dict(color='red', size=8))

    layout = go.Layout(title='Filtered k, Derivative and Extremes',
                    xaxis=dict(title='Time'),
                    yaxis=dict(title='Value'))

    fig = go.Figure([trace_k, trace_filtered_k, trace_derivative, trace_extremes], layout=layout)
    fig.show()
    pio.write_html(fig, 'filtered_k_derivative_extremes.html')

if __name__ == "__main__":
    path = './'  
    # part2(path)
    part3(path)