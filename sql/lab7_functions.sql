USE vv;

GO
CREATE FUNCTION dbo.GetEmployeeFullName (@EmployeeID INT)
RETURNS NVARCHAR(101)
AS
BEGIN
	DECLARE @FullName NVARCHAR(101);

	SELECT @FullName = e.FirstName + N' ' + LastName
	FROM Employees e
	WHERE e.EmployeeID = @EmployeeID;

	RETURN @FullName;
END;


GO
CREATE FUNCTION dbo.GetDepartmentAvgSalary (@DepartmentID INT)
RETURNS DECIMAL(10,2)
AS
BEGIN
	DECLARE @AvgSalary DECIMAL(10,2);

	SELECT @AvgSalary = AVG(e.Salary)
	FROM Employees e
	WHERE e.DepartmentID = @DepartmentID;

	RETURN @AvgSalary;
END;


GO
CREATE FUNCTION dbo.GetEmployeeTotalSales (@EmployeeID INT)
RETURNS DECIMAL(10,2)
AS
BEGIN
	DECLARE @TotalSales DECIMAL(10,2);

	SELECT @TotalSales = SUM(s.TotalAmount)
	FROM Sales s
	WHERE s.EmployeeID = @EmployeeID;

	RETURN @TotalSales;
END;

GO
CREATE FUNCTION dbo.GetStockStatus (@ProductID INT)
RETURNS NVARCHAR(20)
AS
BEGIN
    DECLARE @Quantity INT;
    DECLARE @Status NVARCHAR(20);

    SELECT @Quantity = StockQuantity
    FROM Products
    WHERE ProductID = @ProductID;

    IF @Quantity = 0
		SET @Status = 'Out of Stock'
	ELSE IF @Quantity < 50
		SET @Status = 'Low Stock'
	ELSE IF @Quantity >= 50
		SET @Status = 'in Stock'

    RETURN @Status;
END;
GO

GO
CREATE FUNCTION dbo.GetEmployeeProjects (@EmployeeID INT)
RETURNS TABLE
AS
RETURN
(
    SELECT p.ProjectName, ep.Role
	FROM EmployeeProjects ep
	JOIN Projects p ON p.ProjectID = ep.ProjectID
	WHERE ep.EmployeeID = @EmployeeID
);


GO
CREATE FUNCTION dbo.GetStudentGPA (@StudentID INT)
RETURNS DECIMAL(5,2)
AS
BEGIN
    DECLARE @GPA DECIMAL(5,2);

    SELECT @GPA = AVG(Grade)
    FROM StudentGrades
    WHERE StudentID = @StudentID;

    RETURN ISNULL(@GPA, 0);
END;

GO
CREATE FUNCTION dbo.GetStudentBestCourse (@StudentID INT)
RETURNS TABLE
AS
RETURN
(
    SELECT TOP 1
		sg.StudentID,
		s.FirstName + N' ' + s.LastName AS FllName,
		c.CourseName,
		sg.Grade,
		sg.Semester,
		sg.Year
	FROM StudentGrades sg
	JOIN Students s ON sg.StudentID = s.StudentID
	JOIN Courses c ON c.CourseID = sg.CourseID
	WHERE s.StudentID= @StudentID
	ORDER BY sg.Grade DESC
);

SELECT dbo.GetEmployeeFullName(3);
SELECT dbo.GetDepartmentAvgSalary(2), DepartmentName
FROM Departments
WHERE DepartmentID = 2;
SELECT dbo.GetEmployeeTotalSales(5);
SELECT dbo.GetStockStatus(10);
SELECT * FROM Employees e CROSS APPLY dbo.GetEmployeeProjects(e.EmployeeID) AS p;
SELECT dbo.GetStudentGPA(4);
SELECT * FROM dbo.GetStudentBestCourse(4);