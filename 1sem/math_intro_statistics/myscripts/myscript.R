conf_interval_f <- function(x,y,tq, s1,n1,s2,n2) {
    res <- c( (x - y) + tq * sqrt((s1^2/n1) + (s2^2/n2)), (x - y) - tq * sqrt( (s1^2/n1) + (s2^2/n2)) )
    return(res)
}

tobs_f <- function(x1,x2,delta,s1,s2,n1,n2) {
    res <- ((x1-x2)-delta) / sqrt( (s1^2/n1) + (s2^2/n2) )
    return(res)
}

df_f <- function(s1,s2,n1,n2) {
    res <- ((s1^2 / n1) + (s2^2 / n2))^2 / ( ((s1^2 / n1)^2/(n1-1)) + ((s2^2 / n2)^2/(n2-1)))
    return(res)
}

#t_quantile = qt(procentage, freedom degrees)
#p_value = 2 * (1 - pt(t_obs, freedom degrees))