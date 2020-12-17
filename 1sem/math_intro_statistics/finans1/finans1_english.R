###########################################################################
## Set the working directory

## In RStudio the working directory is easily set via the menu
## "Session -> Set Working Directory -> To Source File Location" 
## Note: In R only "/" is used for separating in paths 
## (i.e. no backslash).

setwd("/home/fluffysnail/dtu/1sem/math_intro_statistics/finans1")

D <- read.table("finans1_data.csv", header=TRUE, sep=";", as.is=TRUE)
## Keep only the dates and the ETFs AGG, VAW, IWN, and SPY

D <- D[ ,c("t","AGG","VAW","IWN","SPY")]

D$t <- as.Date(x=D$t, format="%Y-%m-%d")

#A
head(D)
tail(D)

#B
hist(D$AGG, xlab="Return (AGG)", prob=TRUE)

#C
## Plots of weekly return over time for each of the four ETFs
ylim <- c(-0.2,0.2)
## Plot of weekly return over time for AGG
plot(D$t, D$AGG, type="l", ylim=ylim, xlab="Date", ylab="Return AGG")
## Similar plots for the three other ETFs
plot(D$t, D$VAW, type="l", ylim=ylim, xlab="Date", ylab="Return VAW")
plot(D$t, D$IWN, type="l", ylim=ylim, xlab="Date", ylab="Return IWN")
plot(D$t, D$SPY, type="l", ylim=ylim, xlab="Date", ylab="Return SPY")


#D
## Box plot of weekly returns by ETF
boxplot(D$AGG, D$VAW, D$IWN, D$SPY, names=c("AGG", "VAW", "IWN", "SPY"),
xlab="ETF", ylab="Return")


#E
m1 <- mean(D$AGG)
m2 <- mean(D$VAW)
m3 <- mean(D$IWN)
m4 <- mean(D$SPY)
v1 <- var(D$AGG)
v2 <- var(D$VAW)
v3 <- var(D$IWN)
v4 <- var(D$SPY)
sd1 <- sd(D$AGG)
sd2 <- sd(D$VAW)
sd3 <- sd(D$IWN)
sd4 <- sd(D$SPY)
q_agg <- quantile(D$AGG) #outputs q1, median and q3
q_vaw <- quantile(D$VAW)
q_iwn <- quantile(D$IWN)
q_spy <- quantile(D$SPY)


#F
qqnorm(D$AGG)
qqline(D$AGG)
qqnorm(D$VAW)
qqline(D$VAW)
qqnorm(D$IWN)
qqline(D$IWN)
qqnorm(D$SPY)
qqline(D$SPY)

plot(ecdf(D$AGG), verticals=TRUE)
plot(ecdf(D$VAW), verticals=TRUE)
plot(ecdf(D$IWN), verticals=TRUE)
plot(ecdf(D$SPY), verticals=TRUE)

conf_interval_one_simple_f <- function(sample_mean, tquantile, sample_sd, length) {
  conf_int <- c( (sample_mean - tquantile * (sample_sd/sqrt(length)), (sample_mean + tquantile * (sample_sd/sqrt(length))) ))
  return(conf_int)
}

#for 95% confidence interval, all length are equivilent, so we can just take a random ETFs length
#if we suspect that lengths may vary in the future, one could take the lenghts of each ETF
all_lengths <- length(D$AGG)
tquant <- qt(0.975, df=all_lengths-1)

#used to confirm/deny H0 in F.
conf_int_agg <- conf_interval_one_simple_f(m1, tquant, s1, all_lengths)
conf_int_vaw <- conf_interval_one_simple_f(m2, tquant, s2, all_lengths)
conf_int_iwn <- conf_interval_one_simple_f(m3, tquant, s3, all_lengths)
conf_int_spy <- conf_interval_one_simple_f(m4, tquant, s4, all_lengths)

#mean and variance is found from E

#G
#to find conf_intervals we can simply use conf_int_x in F
#alternatively, we can look at t.test and find observe the confidence interval for each
t.test(D$AGG) 
t.test(D$VAW)
t.test(D$IWN)
t.test(D$SPY)


#H
tobs_f <- function(sample_mean, mu0, sample_sd, length) {
  return( (sample_mean - mu0) / (sample_sd / sqrt(length)))
}
tobs <- tobs_f(m1, 0, sample_sd, all_lengths)
p <- 2* (1 - (pt(tobs, df=all_lengths-1)))
#alternatively
t.test(D$AGG) #read t and p


#I
two_sample_conf_interval_f <- function(x,y,tq, s1,n1,s2,n2) {
    res <- c( (x - y) + tq * sqrt((s1^2/n1) + (s2^2/n2)), (x - y) - tq * sqrt( (s1^2/n1) + (s2^2/n2)) )
    return(res)
}

two_sample_tobs_f <- function(x1,x2,delta,s1,s2,n1,n2) {
    res <- ((x1-x2)-delta) / sqrt( (s1^2/n1) + (s2^2/n2) )
    return(res)
}

two_sample_df_f <- function(s1,s2,n1,n2) {
    res <- ((s1^2 / n1) + (s2^2 / n2))^2 / ( ((s1^2 / n1)^2/(n1-1)) + ((s2^2 / n2)^2/(n2-1)))
    return(res)
}
agg_vaw_tobs <- two_sample_tobs_f(m1,m2,0,s1,s2,all_lengths, all_lengths)
agg_vaw_df <- two_sample_df_f(s1,s2,all_lengths,all_lengths)
agg_vaw_tquant_95 <- qt(0.975, df=agg_vaw_df)
agg_vaw_conf_int <- two_sample_conf_interval_f(m1,m2, agg_vaw_tquant_95, s1, all_lengths, s2, all_lengths)
#is |tobs| < agg_vaw_tquant_95, then accept otherwise reject.
agg_vaw_p <- 2* (1-pt(0.975, df=agg_vaw_df)) #is agg_vaw_p > 0.05 then accept, otherwise reject

#alternatively
#conf.level = 0.95 default, alpha = 0.05 
t.test(D$AGG, D$VAW) #read tobs, p and degrees of freedom

#J
#No Code

#K
cor(D[ ,c("AGG","VAW","IWN","SPY")], use="pairwise.complete.obs")
plot(D$IWN, D$VAW) #scatter plot