#Assignemnt A
D <- read.table("finans2_data.csv", header = TRUE, sep = ";")
D_sel <- D[, c(2, 3, 4)]
names(D_sel) <- c("Geo_mean", "Vol", "Max_tuw")

#scatterplot
pairs(D_sel, panel = panel.smooth, main = "geo mean vs Vol and Max_tuw")

#Boxplots
boxplot(D$Geo.mean, main = "Geometric Average return")
boxplot(D$Volatility, main = "Volatility")
boxplot(D$maxTuw, main = "Max Time Under Water")

#Histogram
hist(D$Geo.mean, main = "Geo.mean")
hist(D$Volatility)
hist(D$maxTuW, main = "Hist Max Time Under Water")

#values for table
sd(D$Volatility)
sd(D$Geo.mean)
sd(D$maxTuW)
summary(D) #length, Q1, Q3, median, mean


#C
D <- read.table("finans2_data.csv", header = TRUE, sep = ";")

# Subset containing only AGG, VAW, IWN and SPY (for validation)
D_test <- subset(D, ETF %in% c("AGG", "VAW", "IWN", "SPY"))

# Subset containing only the 91 remaining ETFs (for model estimation)
D_model <- subset(D, !(ETF %in% c("AGG", "VAW", "IWN", "SPY")))

fit <- lm(Geo.mean ~ Volatility + maxTuW, data = D_model)

# Show parameter estimates etc.
summary(fit)

#D
par(mfrow = c(3, 2))
# Observations against fitted values
plot(fit$fitted.values, D_model$Geo.mean, xlab = "Fitted values",
       ylab = "Geom. average rate of return")

# # Residuals against each of the explanatory variables
# plot(D_model$EXPLANATORY_VARIABLE, fit$residuals,
#         xlab = "INSERT TEXT", ylab = "Residuals")

plot(D_model$Volatility, fit$residuals,
       xlab = "Volatility", ylab = "Residuals")

plot(D_model$maxTuW, fit$residuals,
       xlab = "maxTuw", ylab = "Residuals")

# Residuals against fitted values
plot(fit$fitted.values, fit$residuals, xlab = "Fitted values",
     ylab = "Residuals")

# Normal QQ-plot of the residuals
qqnorm(fit$residuals, ylab = "Residuals", xlab = "Z-scores",
       main = "")
qqline(fit$residuals)

#E
vol_upper <- -0.0351310 + qt(0.975, 88) * 0.0097229
vol_lower <- -0.0351310 - qt(0.975, 88) * 0.0097229
vol_95_int <- c(vol_lower, vol_upper) #should be identitical to confint(fit) for volatility
confint(fit)

#F

#G
fit_vol <- lm(Geo.mean ~ Volatility, data = D_model)


#H
pred <- predict(fit_vol, newdata = D_test, interval = "prediction", level = 0.95)
cbind(id = D_test$ETF, Geo.mean = D_test$Geo.mean, pred)

# Confidence intervals for the model coefficients
#confint(fit, level = 0.95)
t_obs <- (-0.0351310 - (-0.06)) / 0.0097229
p_value <- 2*(1-pt(abs(t_obs),df=94))