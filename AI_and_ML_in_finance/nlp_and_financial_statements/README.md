# Using_NLP_to_Predict_Almost_Bankruptcy
The goal of the project is to see if sharp and extreme equity drawdown , as a proxy for bankruptcy, credit risk and governance, can be predicted from the language of a company’s annual report and to compare this with the performance from more traditional measures using financial metrics.

The model is available in the self-named folder and uses the first expanding window with training data up to and including 2014. It is structured as a function that takes the annual company report as a document input and produces 1 for prediction of an 80% (or greater) 20-day stock price drawdown in the year following the report filing date and 0 otherwise. Have fun!
