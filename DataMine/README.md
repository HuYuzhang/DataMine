# Web Data Mining Homework#1

## Problem Description
Now we have many web's data, and we want to calculate 100W web's PageRank.

>PageRank works by counting the number and quality of links to a page to determine a rough estimate of how important the website is. The underlying assumption is that more important websites are likely to receive more links from other websites.


## How to solve it
* First we scan the raw data and collect the information of titles and references and save them.
* Then after we collect 100W titles, we parse the referenes we have just saved. For the references whose target hasn't been found, we just abandon it.
* Finally, we begin the iteration as defined by PageRank and then sort all pages.

## Comments

### XMLParser
* Read raw data and construct data of pages
* Construct data of pages and build the relationship of each page
* Calculate the PageRank and save the result

```
    void init();//Read data from raw file
    void destroy();//clean all data
    void work();//calculate the PageRank
```

### Page
* Store some information about pages, like name, and reference information
* Update each page's PageRank


## Other
* After the program has used 6G+ memory when sound from my disk, it suddenlly stopped for 5 seconds and then available memory grows.
* Compile in release mode, it will be faster. Here it will cost about 30 min to finish the calculation.
* Build it in Visual Studio 2013, just open then sln file. For source file, they are in the DataMine/DataMine directory
