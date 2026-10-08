--1.
USE NORTHWND
GO
CREATE VIEW View_EmployeeNames
AS
SELECT FirstName, LastName
FROM Employees;
--2.
GO
CREATE VIEW View_MaxFreightPerEmployee
WITH ENCRYPTION
AS
SELECT e.LastName, MAX(o.Freight) AS MaxFreight
FROM Employees e
JOIN Orders o ON e.EmployeeID = o.EmployeeID
GROUP BY e.LastName, e.EmployeeID;
--3.1
GO
CREATE VIEW View_ReportsToFuller
AS
SELECT e1.LastName
FROM Employees e1
JOIN Employees e2 ON e1.ReportsTo = e2.EmployeeID
WHERE e2.LastName = 'Fuller';
--3.2.
GO
CREATE VIEW View_ReportsToFuller
AS
SELECT LastName
FROM Employees
WHERE ReportsTo = (
    SELECT EmployeeID 
    FROM Employees 
    WHERE LastName = 'Fuller'
);
--4.
GO
CREATE VIEW View_DiscountedProducts
AS
SELECT DISTINCT s.CompanyName, p.ProductName
FROM Suppliers s
JOIN Products p ON s.SupplierID = p.SupplierID
JOIN [Order Details] od ON p.ProductID = od.ProductID
WHERE od.Discount > 0;