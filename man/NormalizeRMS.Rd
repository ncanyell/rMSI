% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/normalizations.R
\name{NormalizeRMS}
\alias{NormalizeRMS}
\title{NormalizeRMS: Calculates the RMS normalizatin of each pixel as root mean square all intensities.}
\usage{
NormalizeRMS(img, remove_empty_pixels = FALSE)
}
\arguments{
\item{img}{the rMSI image object.}

\item{remove_empty_pixels}{boolean detailing if pixels detected to not contain data must be removed from normalization (smaller than mean-*sd).}
}
\value{
a rMSI image containing the normalizations$RMS field or RMSne if remove_empty_pixels is true.
}
\description{
NormalizeRMS: Calculates the RMS normalizatin of each pixel as root mean square all intensities.
}
