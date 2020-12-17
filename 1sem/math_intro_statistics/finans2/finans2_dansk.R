# Indlæs 'finans2_data.csv' filen med data
D <- read.table("finans2_data.csv", header = TRUE, sep = ";")




# Deldatasæt med AGG, VAW, IWN og SPY (til validering)
D_test <- subset(D, ETF %in% c("AGG","VAW","IWN","SPY"))

# Deldatasæt med kun de øvrige 91 ETF'er (til model)
D_model <- subset(D, !(ETF %in% c("AGG","VAW","IWN","SPY")))


# Estimer multipel lineær regressionsmodel
fit <- lm(Geo.mean ~ Volatility + maxTuW, data = D_model)

# Vis estimerede parametre mm.
summary(fit)


# Plots til modelkontrol

# Observationer mod fittede værdier
plot(fit$fitted.values, D_model$Geo.mean, xlab = "Fittede værdier",     
       ylab = "Gnsn. rel. ugentligt afkast")

# Residualer mod hver af de forklarende variable
plot(D_model$FORKLARENDE_VARIABEL, fit$residuals, 
        xlab = "INDSÆT TEKST", ylab = "Residualer")

# Residualer mod fittede værdier
plot(fit$fitted.values, fit$residuals, xlab = "Fittede værdier", 
     ylab = "Residualer")

# Normal QQ-plot af residualerne
qqnorm(fit$residuals, ylab = "Residualer", xlab = "Z-scores", 
       main = "")
qqline(fit$residuals)


# Konfidensintervaller for modellens koefficienter
confint(fit, level = 0.95)


# Prædiktioner og 95% prædiktionsintervaller
pred <- predict(SLUTMODEL, newdata = D_test, interval = "prediction", 
              level = 0.95)

# Observerede værdier sammen med prædiktioner
cbind(ETF = D_test$ETF, Geo.mean = D_test$Geo.mean, pred)

