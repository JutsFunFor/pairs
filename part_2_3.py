import os
import numpy as np
import pandas as pd
import plotly.graph_objects as go
import plotly.io as pio
from filterpy.kalman import KalmanFilter
from scipy.signal import find_peaks
from plotly.subplots import make_subplots

class Vis():
    """
        private:
            __load_data - loads csv data in specified dir
            __filter_data - kalman filter 
            __deriviate - find deriviate of input data
            __find_extremes - return extremes indicies of input data
        public:
            part2 - subplots for csv files and K data
            part3 - subplots for K data, filtered K data, derivative, and extremes with lines.
    """
    def __load_data(self, dir_path):
        all_data = {}
        for filename in os.listdir(dir_path):
            if filename.lower().endswith(".csv"):
                data = pd.read_csv(os.path.join(dir_path, filename), header=None, sep=';')
                data.columns = ["timestamp", "price"]
                all_data[filename[:-4]] = data
        return all_data

    def part2(self, path):
        all_data = self.__load_data(path)
        fig = make_subplots(rows=2, cols=2, vertical_spacing=0.1, horizontal_spacing=0.1)

        row, col = 1, 1
        for key, data in all_data.items():
            trace = go.Scatter(x=pd.to_datetime(data['timestamp'], unit='s'), y=data['price'], name='Result' if key == 'out' else key)
            fig.add_trace(trace, row=row, col=col)
            fig.update_xaxes(title_text='Time', row=row, col=col)
            fig.update_yaxes(title_text='Value', row=row, col=col)
            fig.update_layout(title=f'Graph {row * 2 - 1 + col}', showlegend=True)

            if col < 2:
                col += 1
            else:
                row += 1
                col = 1


        fig.update_layout(height=900, width=1200,
                           title_text="Forex Prices and Coefficient k",
                           title_x=0.5)

        fig.show()
        pio.write_html(fig, 'part2.html')


    def __filter_data(self, data):
        kf = KalmanFilter(dim_x=1, dim_z=1)
        kf.x = np.array([data[0]])  
        # err cov
        kf.P *= 0.01 

        kf.F = np.array([[1.]]) 
        kf.H = np.array([[1.]]) 
        # noise cov
        kf.R *= 0.01 
        kf.Q *= 0.001 

        fdata = []
        for measurement in data:
            kf.predict()
            kf.update(measurement)
            fdata.append(kf.x[0])

        return np.array(fdata)


    def __derivative(self, data):
        return np.gradient(data)

    def __find_extremes(self, data):
        idxmax, _ = find_peaks(data, distance=950)
        idxmin, _ = find_peaks(-1*data, distance=950)
        return idxmax, idxmin

    def part3(self, path):
        all_data = self.__load_data(path)
        k_data = all_data['out']['price']
        filtered_data = self.__filter_data(k_data)
        deriv = self.__derivative(filtered_data)
        idxmax, idxmin = self.__find_extremes(filtered_data)

        trace_k = go.Scatter(y=k_data, mode='lines', name='k')
        trace_filtered_k = go.Scatter(y=filtered_data, mode='lines', name='Filtered k', line=dict(color='blue'))
        trace_deriv = go.Scatter(y=deriv, mode='lines', name='Derivative', line=dict(width=2, color='purple'))
        trace_max = go.Scatter(x=idxmax,
                              y=[filtered_data[i] for i in idxmax],
                              mode='markers', name='Max', marker=dict(color='green', size=8))
        trace_min = go.Scatter(x=idxmin,
                               y=[filtered_data[i] for i in idxmin],
                               mode='markers', name='Min', marker=dict(color='red', size=8))
        

        fig = make_subplots(rows=2, cols=1, shared_xaxes=True)

        fig.add_trace(trace_k, row=1, col=1)
        fig.add_trace(trace_filtered_k, row=1, col=1)
        fig.add_trace(trace_max, row=1, col=1)
        fig.add_trace(trace_min, row=1, col=1)

        for idx in idxmax:
            fig.add_shape(type="line", x0=idx, y0=min(k_data), x1=idx, y1=max(k_data), row=1, col=1, line=dict(color='green', width=1, dash='dash'))
            fig.add_shape(type="line", x0=idx, y0=min(deriv), x1=idx, y1=max(deriv), row=2, col=1, line=dict(color='green', width=1, dash='dash'))

        for idx in idxmin:
            fig.add_shape(type="line", x0=idx, y0=min(k_data), x1=idx, y1=max(k_data), row=1, col=1, line=dict(color='orange', width=1, dash='dash'))
            fig.add_shape(type="line", x0=idx, y0=min(deriv), x1=idx, y1=max(deriv), row=2, col=1, line=dict(color='orange', width=1, dash='dash'))

        fig.add_trace(trace_deriv, row=2, col=1)
        fig.update_layout(title='Graphs', xaxis=dict(title='Count'), showlegend=True)
        fig.update_xaxes(title_text='Count', row=2, col=1)
        fig.update_yaxes(title_text='Value', row=1, col=1)
        fig.update_yaxes(title_text='Derivative', row=2, col=1)
        fig.update_layout(legend=dict(
            orientation="h",
            yanchor="bottom",
            y=1.02,
            xanchor="right",
            x=1
        ))

        fig.show()
        pio.write_html(fig, 'part3.html')
if __name__ == "__main__":
    path = './'

    vis = Vis()
    vis.part2(path)  
    vis.part3(path)