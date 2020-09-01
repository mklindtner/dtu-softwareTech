# Read the dataset 'finans2_data.csv' into R
D <- read.table("finans2_data.csv", header = TRUE, sep = ";")

# Subset containing only AGG, VAW, IWN and SPY (for validation)
D_test <- subset(D, ETF %in% c("AGG","VAW","IWN","SPY"))

# Subset containing only the 91 remaining ETFs (for model estimation)
D_model <- subset(D, !(ETF %in% c("AGG","VAW","IWN","SPY")))

# Estimate multiple linear regression model
fit <- lm(Geo.mean ~ Volatility + maxTuW, data = D_model)

# Show parameter estimates etc.
summary(fit)

# Plots for model validation

# Observations against fitted values
plot(fit$fitted.values, D_model$Geo.mean, xlab = "Fitted values",     
       ylab = "Geom. average rate of return")

# Residuals against each of the explanatory variables
plot(D_model$EXPLANATORY_VARIABLE, fit$residuals, 
        xlab = "INSERT TEXT", ylab = "Residuals")

# Residuals against fitted values
plot(fit$fitted.values, fit$residuals, xlab = "Fitted values", 
     ylab = "Residuals")

# Normal QQ-plot of the residuals
qqnorm(fit$residuals, ylab = "Residuals", xlab = "Z-scores", 
       main = "")
qqline(fit$residuals)

# Confidence intervals for the model coefficients
confint(fit, level = 0.95)

# Predictions and 95% prediction intervals
pred <- predict(FINAL_MODEL, newdata = D_test, 
                interval = "prediction", level = 0.95)

# Observed values and predictions
cbind(id = D_test$ETF, Geo.mean = D_test$Geo.mean, pred)

