# COMP6771 Final Exam

## Change Log

This section will be updated as clarifications and changes occur, and each addition will involve direct communication with students via email.

### Question 1
* 7:30am Q1 clarification: Assume only valid inputs
* 7:30am Q1 clarification: No need to consider nested repeat/norepeat statements
* 7:30am Q1 clarification: A fix to the startup code has been made and will be deployed between 7:30am-9:30am to your repo via a pull request. If you need that change urgently you can [view the diff here](https://gitlab.cse.unsw.edu.au/COMP6771/20T2-cs6771-exam/-/commit/c1828fb2b5e80397d23010b01a1a54c3fe329cf6)
* 7:30am Q1 clarification: A lot of people have asked about int/double promotion etc. Please take a look at the example input/output before posting your question on the forum.
* 9:00am Q1 clarification: More information on int/double promotion given in spec. See section 1.3.
* 11:50am Q1 clarification: The number at the top of the stack before a repeat can be assumed to be an positive int (based on the input test cases we will provide).

### Question 2
* 8:30am Q2 clarification: One of the prototypes given in the .hpp file did not match the spec. This has been updated. You can [view the diff here](https://gitlab.cse.unsw.edu.au/COMP6771/20T2-cs6771-exam/-/commit/cdb9ed8c64169a0c47a5222cf880252de5cc8c52)
* 10:00am Q2 clarification: `ridx_` structure clarification. You can [view the diff here](https://gitlab.cse.unsw.edu.au/COMP6771/20T2-cs6771-exam-spec/-/commit/e300862c01637dc657f95dadc17e3807e27d3ff8)
* 12:00pm Q2 clarification: You can assume that element() that returns a reference will not be called on a cell without a non-zero value in it.
* 12:00pm Q2 clarification: "size *k*" clarified to "with *k* elements in it" at two points
* 12:15pm Q2 clarification: You are welcome to add additional functions and data members, as long as you still implement the compulsory data members as specified, and as long as your code still compiles on vlab.
*  2:07pm Q2 clarification: All mutating operations on sparse_matrix invalidate iterators.
*  4:10pm Removed "An oversight in clarifying" typo
*  5:20pm Q2 clarification: Changed "Returns the number of rows" to "Returns the number of columns" for the `cols()` call
*  5:30pm Q2 clarification: Clarified that the iterator moves left-right, top-bottom (like English reading) over non-zero elements. That is what is meant by linear.
*  5:30pm Q2 clarification: `operator*=` previously used to say "LHS" twice (typo), instead of LHS/RHS. This has been fixed.
*  5:45pm Q2 clarification: You do not have to be concerned with the state of iterators after an object is moved.
*  5:45pm Q2 clarification: The static data member will be valid if both public or private.

### Question 3
* 7:30am Q3 clarification: You are only given `students.db` to start with, you will have to generate your own other `.db` files if you wish to test that way.
* 8:00am Q3 clarification: You can assume no new line character `\n` will be part of the data provided for a record on input.
* 8:15am Q3 clarification: You can assume that `operator>>` will only be invoked once per record empty.
* 10:20am Q3 clarification: Added `count` description
* 11:00am Q3 clarification: Change of `database::delete` to `delete_matching`.
* 11:59am Q3 clarification: A few instances of `match` in spec renamed to `matches`.
*  5:30pm Q3 clarification: Added "where the value is" to `query_greater_than` and `query_starts_with`, as this was causing confusion

## Questions

### Question 1

You can see the requirements for question 1 by visiting the [question 1 specification](q1/README.md)

### Question 2

You can see the requirements for question 2 by visiting the [question 2 specification](q2/README.md)

### Question 3

You can see the requirements for question 3 by visiting the [question 3 specification](q3/README.md)

## Marking Criteria

Your performance in your exam will be determined by the result of automated marking. We will compile your program, and run it against a series of tests that assess whether your program behaves as required. You will not know the full sample of tests used prior to marking.

Code that does not compile risks receiving no automarks for that question.

If you submit your work and your tentative grade is nearing a fail of the hurdle, and it's due to trivial and easily-resolvable errors (the definition of easily-resolvable is at our discretion), we may modify your code and run it again.

Code that required manual intervention to achieve a pass mark will result in a maximum exam mark equal to the scaled exam pass mark.

While bad practices are not strictly "prohibited" in the exam, any usage of the following that is more than occasional will result in a of marks:
* Use of C-style pointers (excluding observer-style pointers to smart pointers)
* Explicit use of new/delete
* C-style arrays
* Usage of libraries not used at any point in lecture code or tutorials

The loss of marks arising from non-trivial use of bad practices may result in a discretionary loss of marks, but no mark reduction of this kind will result in a fail of the exam.

## Submission

When you've completed your work, please ensure you push your most recent code changes to your master branch on gitlab. Go to the gitlab GUI and make sure your most recent work is there.

Then run this command on the CSE systems.

`6771 submit exam`

Please note: Remember to ensure your code works on the UNSW CSE machines. If you develop on Vlab this is probably fine, but if you've developed locally ensure it runs on vlab OK. Failure to do so could result in a fail mark in the exam.

## Sanity checking

If you want to check if you've actually submitted your questions, and see if they pass a very basic compilation example, then run this command on the CSE systems.

`6771 examcheck`

## Originality of Work

The work you submit must be your own work. Submission of work partially or completely derived from any other person or jointly written with any other person is not permitted.

The penalties for such an offence may include negative marks, automatic failure of the course and possibly other academic discipline. Assignment submissions will be examined both automatically and manually for such submissions.

Relevant scholarship authorities will be informed if students holding scholarships are involved in an incident of plagiarism or other misconduct.

Do not provide or show your assignment work to any other person — apart from the teaching staff of COMP6771.

If you knowingly provide or show your assignment work to another person for any reason, and work derived from it is submitted, you may be penalized, even if the work was submitted without your knowledge or consent.  This may apply even if your work is submitted by a third party unknown to you.

Note you will not be penalized if your work has the potential to be taken without your consent or
knowledge.

## Other information

If you are seeking other information, first check the [COMP6771 Exam Brief page on Webcms3](https://webcms3.cse.unsw.edu.au/COMP6771/20T2/resources/50133), and if your question is unanswered, follow the "Communicating with teaching staff" instructions on said link.
