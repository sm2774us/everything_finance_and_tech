#Algorithmic Trading with Machine Learning

#imports
from time import *
from sklearn import tree
import datetime as dt
import matplotlib.pyplot as plt
from matplotlib import style
import pandas as pd
import pandas_datareader.data as web
import time
start_time = time.time()
import warnings
warnings.filterwarnings("ignore",category=DeprecationWarning)
    

#trading algorithm
def algo(t):

    features = []
    labels = []

    for i in range(len(t) - acc + 1):
        features.append(t[-1*acc:-1])

        #1 means price went up
        if t[-1] > t[-2]:
            labels.append(1)
        else:
            labels.append(0)
            
    clf = tree.DecisionTreeClassifier()
    clf.fit(features, labels)

    if clf.predict(t[-1*acc+1:])[0] == 1:
        return 1
    else:
        return 0

#fields    
acc = 10
Points = []
dates = []
CashRecords = []

Cash = 100
Bought = False
days = 0
decision = 0
stockSymbol = 'AAPL'

style.use('ggplot')
start = dt.datetime(2015,1,1)
end = dt.datetime(2016,12,31)

#importing data
df = web.DataReader(stockSymbol,'google',start,end)
df.to_csv('data.csv')

df = pd.read_csv('data.csv', parse_dates = True)
for i in df[['Close']]:
    count = 0
    for j in df[i]:
        Points.append(round(j,2))

for i in df[['Date']]:
    count = 0
    for j in df[i]:
        dates.append(dt.datetime.strptime(j, "%Y-%m-%d"))

#graph labels        
plt.figure(num = stockSymbol)
plt.title(stockSymbol + " Stock Algorithmic Trading Analysis")
plt.xlabel('Date')
plt.ylabel('Stock Price / Cash')

while days <= len(df[['Close']]) - 1:
    
    #stock info
    days += 1
    StockPrice = Points[days - 1]
    
    if days == 1:
        initP = StockPrice
        initC = Cash
        
    #your money
    if Bought == True:
        Cash = round(Cash*StockPrice/Points[days-2],2)
        c = "green"
    else:
        c = "red"
                  
    CashRecords.append(Cash)
    
    if days > acc:
        decision = algo(Points[:days])

    if Bought == True:
        if decision == 0:
            Bought = False
    else:
        if decision == 1:
            Bought = True

    
    plt.plot(dates[days - 2:days], Points[days - 2:days], color=c)
    
print("Ending Cash: " + str(CashRecords[-1]))
print("Expected Cash: " + str(round(CashRecords[0] * Points[-1] / Points[0],2)))
print("Performance: " + str(round(100 * CashRecords[-1] * Points[0] / (Points[-1] * CashRecords[0]),2)) + "%")

plt.plot(dates, CashRecords, color='blue')
plt.show()
