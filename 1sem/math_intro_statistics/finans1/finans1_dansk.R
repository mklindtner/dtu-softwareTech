
###########################################################################
## Sæt working directory

## I RStudio kan man nemt sætte working directory med menuen 
## "Session -> Set Working Directory -> To Source File Location" 
## Bemærk: i R bruges kun "/" til separering i stier 
## (altså ingen backslash).
setwd("Erstat her med stien til den mappe, hvor projektfilerne er gemt.")

###########################################################################
## Indlæs data

## Indlæs data fra finans1_data.csv
D <- read.table("finans1_data.csv", header=TRUE, sep=";", as.is=TRUE)

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

## Antal observationer af AGG afkast
## (medregner ej eventuelle manglende værdier)
sum(!is.na(D$AGG))
## Stikprøvegennemsnit for AGG afkast
mean(D$AGG, na.rm=TRUE)
## Stikprøvevarians for AGG afkast
var(D$AGG, na.rm=TRUE)
## osv.
##
## Argumentet 'na.rm=TRUE' sørger for at størrelsen
## udregnes selvom der eventuelt er manglende værdier

###########################################################################
## qq-plot til modelkontrol

## qq-plot for AGG afkast
qqnorm(D$AGG)
qqline(D$AGG)

###########################################################################
## Konfidensinterval for middelværdi

## Konfidensinterval for middelafkast for AGG
t.test(D$AGG, conf.level=0.95)$conf.int

###########################################################################
## T-test for en enkelt stikprøve

## Test af hypotesen mu=0 for AGG afkast
t.test(D$AGG, mu=0)

###########################################################################
## Welch t-test for sammenligning af to (uafhængige) stikprøver

## Sammenligning af afkast for VAW og AGG
t.test(D$VAW, D$AGG)

###########################################################################
## Beregning af korrelation

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
## Flere R-tips

## Lav en for-løkke med beregning af et par opsummerende størrelser
## og gem resultatet i en ny data.frame
num <- 2:5
Tbl <- data.frame()
for(i in num){
  Tbl[i-1,"mean"] <- mean(D[ ,i])
  Tbl[i-1,"var"] <- var(D[ ,i])
}
row.names(Tbl) <- names(D)[num]
## Se hvad der er i Tbl
Tbl

## I R er der endnu mere kortfattede måder sådanne udregninger kan 
## udføres. For eksempel
apply(D[, num], 2, mean, na.rm=TRUE)
## eller flere ad gangen i et kald
apply(D[, num], 2, function(x){
  c(mean=mean(x, na.rm=TRUE),
    var=var(x, na.rm=TRUE))
})
## Se flere smarte funktioner med: ?apply, ?aggregate og ?lapply
## og for ekstremt effektiv databehandling se f.eks. pakkerne: dplyr,
## tidyr, reshape2 og ggplot2.

## LaTeX tips:
##
## R-pakken "xtable" kan generere LaTeX tabeller og skrive dem direkte 
## ind i en fil, som derefter kan inkluderes i et .tex dokument.
## 
## R-pakken "knitr" kan anvendes meget elegant til at lave et .tex 
## dokument der inkluderer R koden direkte i dokumentet. Dette 
## dokument og bogen er lavet med knitr.
