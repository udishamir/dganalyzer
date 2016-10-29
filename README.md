dganalyzer try to detect domain generation algorithms (DGA).
============================================================
[Wiki](https://en.wikipedia.org/wiki/Domain_generation_algorithm) DGA are algorithms seen in various families of malware that are used to periodically generate a large number of domain names that can be used as rendezvous points with their command and control servers. The large number of potential rendezvous points makes it difficult for law enforcement to effectively shut down botnets, since infected computers will attempt to contact some of these domain names every day to receive updates or commands. By using public-key cryptography, it is unfeasible for law enforcement and other actors to mimic commands from the malware controllers as some worms will automatically reject any updates not signed by the malware controllers.

I started with the [Domain-Analyzer](https://github.com/udishamir/Domain-Analyzer) project few years ego in order to try and help in detecting malicious domains/ip address by inspecting the  malicious autonomous systems numbers  (ASN) and by meassuring the responce header i get from the domain address home page (if any). While i think its solid idea updating the ASN list is tedious and not always effective.  

## DGA 
We know them when we see them but in order to detect them programmaticaly im using edit distance algorithm to try and detecting names that make "no sense".

