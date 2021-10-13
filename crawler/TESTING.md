# Salifyanjin J Namwila
## CS50 Fall 2021, Lab 4: TSE Cralwer Testing

### Testing
Crawler is tested by running `testing.sh` (integration testing) from the command line after `crawler` has been assembled. `testing.sh` tests the crawler's response to invalid arguments, an incorrect number of arguments,
different depths, and different seed URLs. To confirm that crawlere has crawled the bag of webpages successfully, `testing.sh` checks if the correct number of files(document_IDs) have been created in the input `pageDirectory`. 

To ensure that no pages are missing or duplicated, `crawler` also prints the following "progress" indicators as it proceeds:
  
  1. Saving URL: `URL`
  2. Exploring URL: `URL` 
  2. Scanning URL: `URL`
  3. Found new URL: `URL`
  4. Found seen URL: `URL`
  5. Found external URL: `URL`

### Limitations
The user must manually scan the webpages to determine if they were explored correctly, which gets acutely tedious with more pages explored. The user may also run the example cases listed [here] http://cs50tse.cs.dartmouth.edu/tse-output from the command line and compare the crawler output with those examples. The user should keep in mind that crawler may  process URLS in different order(items from bag are extracted randomly). Ergo, user's directory may not be identical to the one on the site listed
