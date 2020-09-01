setwd("C:/Users/Kuzin/Google Drive/DTU/02_Semester/Statistics/finans2")

# Indl√¶s 'finans2_data.csv' filen med data
D <- read.table("finans2_data.csv", header = TRUE, sep = ";")

# DeldatasÊt med AGG, VAW, IWN og SPY (til validering)
D_test <- subset(D, ETF %in% c("AGG","VAW","IWN","SPY"))

# DeldatasÊt med kun de ¯vrige 91 ETF'er (til model)
D_model <- subset(D, !(ETF %in% c("AGG","VAW","IWN","SPY")))

# A)
plot(D$Volatility,D$Geo.mean, xlab="Volatility",ylab="Geo Mean")
plot(D$maxTuW,D$Geo.mean, xlab="Maximum Time Under Water",ylab="Geo Mean")

hist(D$Geo.mean)
hist(D$Volatility)
hist(D$maxTuW)

boxplot(D$Volatility, ylab="Volatility")
boxplot(D$maxTuW, ylab="Maximum Time under Water")
boxplot(D$Geo.mean, ylab="Geo mean")

summary(D$Geo.mean)
summary(D$Volatility)
summary(D$maxTuW)

# C)

# Estimer multipel line√¶r regressionsmodel
fit <- lm(Geo.mean ~ Volatility + maxTuW, data = D_model)

# Vis estimerede parametre mm.
summary(fit)

# D)

# Plots til modelkontrol

# Observationer mod fittede vÊrdier
#plot(fit$fitted.values, D_model$Geo.mean, xlab = "Fittede vÊrdier",     
#       ylab = "Gnsn. rel. ugentligt afkast")

# Residualer mod fittede v√¶rdier
plot(fit$fitted.values, fit$residuals, xlab = "Fittede vÊrdier", 
     ylab = "Residualer")

# Normal QQ-plot af residualerne
qqnorm(fit$residuals, ylab = "Residualer", xlab = "Z-scores", 
       main = "")
qqline(fit$residuals)

# Residualer mod hver af de forklarende variable
plot(D_model$Volatility, fit$residuals, 
        xlab = "Volatility", ylab = "Residualer")
# Residualer mod hver af de forklarende variable
plot(D_model$maxTuW, fit$residuals, 
     xlab = "Maximum Time Under Water", ylab = "Residualer")



# E)
-0.035-qt(0.975,88)*0.0097
-0.035+qt(0.975,88)*0.0097

# Konfidensintervaller for modellens koefficienter
confint(fit, level = 0.95)

# F)
(-0.035-(-0.06))/0.0097
2*(1-pt(0.975,88))

# G)
# Ny liste, som er valtility kvadreret
VolatilitySq <- D_model$Volatility^2

# Den nye linear model
SLUTMODEL <- lm(Geo.mean ~ Volatility + VolatilitySq, data = D_model)
summary(SLUTMODEL)
# Tilf¯j dataen 
D_test$VolatilitySq = D_test$Volatility^2

# H)

# Pr√¶diktioner og 95% pr√¶diktionsintervaller
pred <- predict(SLUTMODEL, newdata = D_test, interval = "prediction", level = 0.95)

# Observerede v√¶rdier sammen med pr√¶diktioner
cbind(ETF = D_test$ETF, Geo.mean = D_test$Geo.mean, pred)

