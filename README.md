dganalyzer try to detect domain generation algorithms (DGA).
============================================================
[Wiki](https://en.wikipedia.org/wiki/Domain_generation_algorithm) DGA are algorithms seen in various families of malware that are used to periodically generate a large number of domain names that can be used as rendezvous points with their command and control servers. The large number of potential rendezvous points makes it difficult for law enforcement to effectively shut down botnets, since infected computers will attempt to contact some of these domain names every day to receive updates or commands. By using public-key cryptography, it is unfeasible for law enforcement and other actors to mimic commands from the malware controllers as some worms will automatically reject any updates not signed by the malware controllers.

I started with the [Domain-Analyzer](https://github.com/udishamir/Domain-Analyzer) project few years ego in order to try and help in detecting malicious domains/ip address by inspecting the  malicious autonomous systems numbers  (ASN) and by meassuring the responce header i get from the domain address home page (if any). While i think its solid idea updating the ASN list is tedious and not always effective.  

## DGA 
We know them when we see them but in order to detect them programmaticaly im using edit distance algorithm to try and detecting names that make "no sense".

dont ask me why i've called it bogon :)

## CACHE
I moved to BerkleyDB in order to have persistence. BerkleyDB is very fast and have solid [API's](https://docs.oracle.com/cd/E17076_02/html/gsg/C/BerkeleyDB-Core-C-GSG.pdf)
obtaining BerkleyDB from:[Here](http://www.oracle.com/technetwork/database/database-technologies/berkeleydb/downloads/index.html).

The MIT License (MIT)
Copyright (c) <2016> <copyright holders>

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
