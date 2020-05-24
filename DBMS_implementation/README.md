# DBMS Implementation 数据库 2020/T1 (COMP9315)
## Overview and Links 
This course aims to introduce students to the detailed internal structure of database management systems (DBMSs) such as Oracle or SQL Server. DBMSs contain a variety of interesting data structures and algorithms that are also potentially useful outside the DBMS context; knowing about them is a useful way of extending your general programming background. While the focus is on relational DBMSs, given that they have the best-developed technological foundation, we will also consider more recent developments in the management of large data repositories.<br/>
[Course Website 课程网站](https://webcms3.cse.unsw.edu.au/COMP9315/20T1/outline)<br/>
[Introduction to PostgreSQL physical storage](http://rachbelaid.com/introduction-to-postgres-physical-storage/)<br/>


## Project
* ass1(New PostgreSQL Data Type): <br/>
Adding a PersonName Data Type to PostgreSQL. 
如何在PostgreSQL加上新的数据类型。<br/>
[Writing Postgres Extensions - the Basics](http://big-elephants.com/2015-10/writing-postgres-extensions-part-i/)<br/>

* ass2(SIMC Signature Index Files):<br/>
 Building a simple implementation of a SIMC signature file, including applications to create SIMC files, insert tuples into them, and search for tuples based on partial-match retrieval queries. <br/>
 实现Tuple Signatures, Page Signatures 和 Bit-sliced Signatures 去进行index-selection的加速。<br/>
（主要代码：bits.c bsig.c page.c psig.c query.c reln.c tsig.c util.c）


## Topics
1. DBMS Revision, PostgreSQL
2. Storage: Devices, Files, Pages, Tuples, Buffers, Catalogs
3. Cost Models, Implementing Scan, Sort, Projection
4. Implementing Selection on One Attribute
5. Implementing Selection on Multiple Attributes
6. Similarity-based Selection
7. Implementing Join
8. Query Translation, Optimisation, Execution
9. Transactions, Concurrency, Recovery
10. Non-classical DBMSs

