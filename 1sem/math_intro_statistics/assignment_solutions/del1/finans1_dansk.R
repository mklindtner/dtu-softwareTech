
###########################################################################
setwd("C:/Users/Kuzin/Google Drive/DTU/02_Semester/Statistics/finans1")

###########################################################################
## Indlæs data

## Indlæs data fra finans1_data.csv
D <- read.table("finans1_data.csv", header=TRUE, sep=";", as.is=TRUE)
##Vi fjerner v�rdierne, hvoraf dataen er 0
D[D == 0] <- NA
## Udvælg tids-variablen samt ETF'erne AGG, VAW, IWN og SPY
D <- D[ ,c("t","AGG","VAW","IWN","SPY")]

###########################################################################
## Simpel opsummering af data

## Dimensionen af D (antallet af rækker og søjler)
dim(D)
## Søjle-/variabelnavne
names(D)
## De første rækker/observationer
head(D)
## De sidste rækker/observationer
tail(D)
## Udvalgte opsummerende størrelser
summary(D)
## En anden type opsummering af datasættet
str(D)

###########################################################################
## Histogram (empirisk tæthed)

## Histogram der beskriver den empiriske tæthed for afkastene for AGG
## (histogram for AGG-afkast normaliseret så arealet er lig 1)
hist(D$AGG, xlab="Afkast (AGG)", prob=TRUE)

###########################################################################
## Konverter variabel til dato

## Konverterer variablen 't' til en dato-variabel i R
D$t <- as.Date(x=D$t, format="%Y-%m-%d")
## Tjekker resultatet
summary(D$t)

###########################################################################
## Plot af udvikling over tid

## Plot af ugentligt afkast over tid for hver af de 4 ETF'er
y=c(-0.2,0.2)
## Plot af det ugentlige afkast for ETF'en AGG over tid
plot(D$t, D$AGG, type="l", ylim=y, xlab="Tid", ylab="Afkast (AGG)")
## Tilsvarende plots for de tre andre ETF'er
plot(D$t, D$VAW, type="l", ylim=y, xlab="Tid", ylab="Afkast (VAW)")
plot(D$t, D$IWN, type="l", ylim=y, xlab="Tid", ylab="Afkast (IWN)")
plot(D$t, D$SPY, type="l", ylim=y, xlab="Tid", ylab="Afkast (SPY)")

###########################################################################
## Boxplot opdelt efter ETF

## Boxplot af afkast opdelt efter ETF
boxplot(D$AGG, D$VAW, D$IWN, D$SPY, names=c("AGG", "VAW", "IWN", "SPY"),
        xlab="ETF", ylab="Afkast")

###########################################################################
## Opsummerende størrelser for AGG afkast

get_summary <- function(set) {
  print(sum(!is.na(set)))
  print(mean(set, na.rm=TRUE))
  print(var(set, na.rm=TRUE))
  print(sd(set, na.rm=TRUE))
  print(quantile(set,type=2, na.rm=TRUE))
}
#AGG
get_summary(D$AGG)
#VAW
get_summary(D$VAW)
#IWN
get_summary(D$IWN)
#SPY
get_summary(D$SPY)

## Argumentet 'na.rm=TRUE' sørger for at størrelsen
## udregnes selvom der eventuelt er manglende værdier

###########################################################################
## qq-plot til modelkontrol

## qq-plot for AGG afkast
qqnorm(D$AGG)
qqline(D$AGG)

###########################################################################
## Konfidensinterval for middelværdi

## Konfidensinterval funktion
confidence_intervals <- function(set) {
  c(mean(set, na.rm=TRUE)-qt(0.975,sum(!is.na(set))-1)*sd(set, na.rm=TRUE)/sqrt(sum(!is.na(set))),
    mean(set, na.rm=TRUE)+qt(0.975,sum(!is.na(set))-1)*sd(set, na.rm=TRUE)/sqrt(sum(!is.na(set))))
}
#Resultat for hver af ETF'erne
confidence_intervals(D$AGG)
t.test(D$AGG, conf.level=0.95)$conf.int
confidence_intervals(D$VAW)
t.test(D$VAW, conf.level=0.95)$conf.int
confidence_intervals(D$IWN)
t.test(D$IWN, conf.level=0.95)$conf.int
confidence_intervals(D$SPY)
t.test(D$SPY, conf.level=0.95)$conf.int


###########################################################################
## T-test for en enkelt stikprøve

## Funktion for at udregne p-value. Udarbejdet udfra eksempel 3.27 i statistikbogen.
pvalue <- function(set) {
  2*(1-pt(abs((mean(set,na.rm=TRUE)-0)/(sd(set,na.rm=TRUE)/sqrt(sum(!is.na(set))))),df=sum(!is.na(set))-1))
}
## Test af hypotesen mu=0 for AGG afkast
pvalue(D$AGG)
t.test(D$AGG, mu=0)
## Test af hypotesen mu=0 for VAW afkast
pvalue(D$VAW)
t.test(D$VAW, mu=0)
## Test af hypotesen mu=0 for IWN afkast
pvalue(D$IWN)
t.test(D$IWN, mu=0)
## Test af hypotesen mu=0 for SPY afkast
pvalue(D$SPY)
t.test(D$SPY, mu=0)

###########################################################################
## Welch t-test for sammenligning af to (uafhængige) stikprøver

## Sammenligning af afkast for VAW og AGG
get_p_value_double <- function(dataset1, dataset2) {
  d1 <- dataset1[!is.na(dataset1)];
  d2 <- dataset2[!is.na(dataset2)];
  n1 <- length(d1);
  n2 <- length(d2);
  v1 <- sd(d1)^2;
  v2 <- sd(d2)^2;
  
  t_obs <- ((mean(d1) - mean(d2)) - 0)/sqrt(v1/n1 + v2/n2);
  v <- ((v1/n1+v2/n2)^2)/((((v1/n1)^2)/(n1-1)) + (((v2/n2)^2)/(n2-1)));
  2 * (1 - pt(abs(t_obs), df=v));
}
get_p_value_double(D$VAW,D$AGG)
t.test(D$VAW, D$AGG)


###########################################################################
## Beregning af korrelation
D <- read.table("finans1_data.csv", header=TRUE, sep=";", as.is=TRUE)
D <- D[ ,c("t","AGG","VAW","IWN","SPY")]
# Funktion til at beregne korrelationen

get_correlations <- function() {
  dimension <- length(names(D)) - 1;
  correlation_matrix <- matrix(nrow=dimension, ncol=dimension);
  c_index <- 1;
  
  for (i in names(D)[-1]){
    correlations <- c();
    index<-1
    
    for (j in names(D)[-1]){
      correlations[index] = (get_correlation(D[,i],D[,j]));
      index<-index+1
    }
    
    correlation_matrix[c_index,] = correlations;
    c_index <- c_index + 1;
  }
  
  colnames(correlation_matrix) <- names(D)[-1];
  rownames(correlation_matrix) <- names(D)[-1];
  correlation_matrix
}
get_correlations()
  
## Beregning af korrelation mellem udvalgte ETF'er
cor(D[ ,c("AGG","VAW","IWN","SPY")], use="pairwise.complete.obs")
  
###########################################################################
## Delmængder i R

## Ekstra bemærkning om måder at udtage delmænger i R
##
## En logisk (logical) vektor med sandt (TRUE) eller falsk (FALSE) for 
## hver række i D - f.eks: 
## De uger hvor der er tab (negativ afkast) på AGG
D$AGG < 0
## Vektoren kan bruges til at udvælge alle de negative AGG afkast
D$AGG[D$AGG < 0]
## Alternativt kan man bruge funktionen 'subset'
subset(D, AGG < 0)
## Mere komplekse logiske udtryk kan laves, f.eks.:
## Find alle observationer fra 2009
subset(D, "2009-01-01" < t & t < "2010-01-01")

###########################################################################
