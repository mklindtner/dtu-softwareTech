two_sample_df <- function(s1, s2, n1, n2) {
  upper <- ((s1 ^ 2 / n1) + (s2 ^ 2 / n2)) ^ 2
  lower <- ((s1 ^ 2 / n2) ^ 2 / (n1 - 1) + (s2 ^ 2 / n2) ^ 2 / (n2 - 1))
  result <- upper / lower
  return(result)
}

two_sample_t_obs <- function(x1, s1, n1, x2, s2, n2, delta) {
    result <- ((x1 - x2) - delta)/sqrt((s1^2/n1) + (s2^2/n2))
    return(result)
}