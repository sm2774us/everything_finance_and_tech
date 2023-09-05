#!/usr/bin/env python3

import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from matplotlib import animation
from pandas.plotting import register_matplotlib_converters
import matplotlib.dates as mdate

register_matplotlib_converters()

fig, (feed_chart, ob_chart) = plt.subplots(2,figsize=(15,5))
ob_chart.grid()
feed_chart.autoscale(tight=True)
ob_chart.autoscale(tight=True)

def fetch_prices(i):
    try:
        ob = pd.read_csv("./data/snapshot.csv", header=None, error_bad_lines=False, warn_bad_lines=False)
        feed = pd.read_csv("./data/feed.csv", header=None, error_bad_lines=False, warn_bad_lines=False, parse_dates=[0])
        feed[0] = pd.to_datetime(feed[0], unit='ms')
        
        quotes = feed[2]
        
        bid_side = ob[ob[0] == 0]
        ask_side = ob[ob[0] == 1]
        
        feed_chart.clear()
        ob_chart.clear()
        date_fmt = '%H:%M:%S'
        date_formatter = mdate.DateFormatter(date_fmt)
        feed_chart.xaxis.set_major_formatter(date_formatter)
        feed_chart.xaxis_date()
        feed_chart.plot(feed[0], quotes, color='green', label='Sell',lw=0.6)
        ob_chart.bar(ask_side[1], ask_side[2], align='center', width=0.0001, color='red', label='Sell')
        ob_chart.bar(bid_side[1], bid_side[2], align='center', width=0.0001, color='green', label='Buy')
    except:
        feed_chart.clear()
        ob_chart.clear()
        print('Broken file - retrying...')
    
fig.autofmt_xdate()
ani = animation.FuncAnimation(fig, fetch_prices, frames=100, interval=200)
plt.show()