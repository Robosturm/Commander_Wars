The following types for Pipeline trainings are allowed

CO -> uses the COID as input
UNIT -> uses the UNITID as input
BUILDING -> uses the BUILDINGID as input
NUMBER compares to numbers for a decision tree the following questions can be generated from the file
	NUMBER Options 
	For greater then NUMBER:0_<
	For Smaller then NUMBER:0_>
	For In Range then NUMBER:0_<_8
	For Equal then NUMBER:1_= 
	For a Combination: NUMBER:<_5|5_<_8|8_= means smaller 5 or between 5 and 8 or equal 8
	For reading the questions from a file use NUMBERFILE:FILEPATH -> were the file path is relative to the commander wars.exe
	Missing questions will be replaced with an isEqual question	