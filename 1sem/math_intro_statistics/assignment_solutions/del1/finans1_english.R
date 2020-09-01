
###########################################################################
## Set the working directory

## In RStudio the working directory is easily set via the menu
## "Session -> Set Working Directory -> To Source File Location" 
## Note: In R only "/" is used for separating in paths 
## (i.e. no backslash).

setwd("C:/Users/Kuzin/Google Drive/DTU/02_Semester/Statistics/finans1")
###########################################################################
## Read data into R

## Read data from finans1_data.csv
D <- read.table("finans1_data.csv", header=TRUE, sep=";", as.is=TRUE)
## Keep only the dates and the ETFs AGG, VAW, IWN, and SPY
D <- D[ ,c("t","AGG","VAW","IWN","SPY")]

###########################################################################
## Simple overview of the data

## Dimensions of D (number of rows and columns)
dim(D)
##  Column/variable names
names(D)
## The first rows/observations
head(D)
## The last rows/observations
tail(D)
## Selected summary statistics
summary(D)
## Another type of summary of the dataset
str(D)

###########################################################################
## Histogram (empirical density)

## Histogram describing the empirical density of the weekly returns from
## AGG (histogram of weekly returns normalized to have an area of 1)
hist(D$AGG, xlab="Return (AGG)", prob=TRUE)

###########################################################################
## Conversion to a date variable

## Converts the variable 't' to a date variable in R
D$t <- as.Date(x=D$t, format="%Y-%m-%d")
## Checks the result
summary(D$t)

###########################################################################
## Plots of data over time

## Plots of weekly return over time for each of the four ETFs
ylim <- c(-0.2,0.2)
## Plot of weekly return over time for AGG
plot(D$t, D$AGG, type="l", ylim=ylim, xlab="Date", ylab="Return AGG")
## Similar plots for the three other ETFs
plot(D$t, D$VAW, type="l", ylim=ylim, xlab="Date", ylab="Return VAW")
plot(D$t, D$IWN, type="l", ylim=ylim, xlab="Date", ylab="Return IWN")
plot(D$t, D$SPY, type="l", ylim=ylim, xlab="Date", ylab="Return SPY")

###########################################################################
## Box plot

## Box plot of weekly returns by ETF
boxplot(D$AGG, D$VAW, D$IWN, D$SPY, names=c("AGG", "VAW", "IWN", "SPY"),
        xlab="ETF", ylab="Return")

###########################################################################
## Summary statistics for weekly AGG returns

## Total number of observations
## (doesn't include missing values if there are any)
sum(!is.na(D$AGG))
## Sample mean for weekly returns from AGG
mean(D$AGG, na.rm=TRUE)
## Sample variance for weekly returns from AGG
var(D$AGG, na.rm=TRUE)
## etc.
sum(!is.na(D$AGG))
mean(D$AGG, na.rm=TRUE)
var(D$AGG, na.rm=TRUE)
sd(D$AGG, na.rm=TRUE)
quantile(D$AGG, na.rm=TRUE,type=2)
sum(!is.na(D$VAW))
mean(D$VAW, na.rm=TRUE)
var(D$VAW, na.rm=TRUE)
sd(D$VAW, na.rm=TRUE)
quantile(D$VAW, na.rm=TRUE,type=2)
sum(!is.na(D$IWN))
mean(D$IWN, na.rm=TRUE)
var(D$IWN, na.rm=TRUE)
sd(D$IWN, na.rm=TRUE)
quantile(D$IWN, na.rm=TRUE,type=2)
sum(!is.na(D$SPY))
mean(D$SPY, na.rm=TRUE)
var(D$SPY, na.rm=TRUE)
sd(D$SPY, na.rm=TRUE)
quantile(D$SPY, na.rm=TRUE,type=2)
## The argument 'na.rm=TRUE' ensures that the statistic is
## computed even in cases where there are missing values.

###########################################################################
## qq-plot for model validation

## qq-plot of AGG's weekly returns
qqnorm(D$AGG)
qqline(D$AGG)

###########################################################################
## Confidence interval for the mean

## CI for the mean weekly return from AGG
t.test(D$AGG, conf.level=0.95)$conf.int

###########################################################################
## One-sample t-test

## Testing the hypothesis mu=0 for weekly AGG returns
t.test(D$AGG, mu=0)

###########################################################################
## Welch t-test for comparing two (independent) samples

## Comparing the mean weekly returns from VAW and AGG
t.test(D$VAW, D$AGG)

###########################################################################
## Computing correlations

## Computing the correlation between selected ETFs
cor(D[ ,c("AGG","VAW","IWN","SPY")], use="pairwise.complete.obs")

###########################################################################
## Subsets in R
  
## Optional extra remark about taking subsets in R
##
## A logical vector with a TRUE or FALSE for row value in D.
## E.g.: The weeks with losses (negative returns) from AGG
D$AGG < 0
## Can be used to extract all AGG losses
D$AGG[D$AGG < 0]
## Alternatively, use the 'subset' function
subset(D, AGG < 0)
## More complex logical expressions can be made, e.g.:
## Find all observations from 2009
subset(D, "2009-01-01" < t & t < "2010-01-01")

###########################################################################
## More R tips

## Use a 'for'-loop to calculate the summary statistics
## and assign the result to a new data.frame
num <- 2:5
Tbl <- data.frame()
for(i in num){
  Tbl[i-1,"mean"] <- mean(D[ ,i])
  Tbl[i-1,"var"] <- var(D[ ,i])
}
row.names(Tbl) <- names(D)[num]
## View the contents of Tbl
Tbl

## In R there are even more condensed ways to perform such 
## calculations, e.g.:
apply(D[, num], 2, mean, na.rm=TRUE)
## or several calculations in one expression
apply(D[, num], 2, function(x){
  c(mean=mean(x, na.rm=TRUE),
    var=var(x, na.rm=TRUE))
})
## See more useful functions with: ?apply, ?aggregate and ?lapply
## For extremely efficient data handling see, e.g., the packages: 
## dplyr, tidyr, reshape2 and ggplot2

## LaTeX tips:
##
## The R package "xtable" can generate LaTeX tables written to a file 
## and thereby they can automatically be included in a .tex document.
## 
## The R package "knitr" can be used very elegantly to generate .tex 
## documents with R code written directly in the document. This  
## document and the book were generated using knitr.

