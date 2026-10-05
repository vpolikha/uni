use NORTHWND

/*
GO
CREATE PROC GetChProd
AS
BEGIN
SELECT p.ProductName
FROM Products p
WHERE left(p.ProductName, 2) = 'Ch' and
p.UnitPrice <= 20
END;
*/


/*
EXEC GetChProd
*/

/*
GO
CREATE PROC GetSumProd
WITH ENCRYPTION
AS
BEGIN
SELECT p.ProductName, p.UnitPrice, p.UnitsInStock, p.UnitPrice * p.UnitsInStock AS gumar
FROM Products p
WHERE p.UnitsInStock != 0
END;
*/

/*
EXEC GetSumProd
*/
/*
GO
CREATE PROC CustByPeriod
	@StartDate DATE,
	@EndGame DATE
AS
BEGIN
	SELECT DISTINCT c.ContactName
	FROM Customers c
	JOIN Orders o
	ON c.CustomerID = o.CustomerID
	WHERE o.OrderDate BETWEEN @StartDate AND @EndGame
END;
*/
/*
EXEC CustByPeriod '1997-01-01', '1997-12-31';
*/
/*
GO
CREATE PROC GetTitle
	@Id NCHAR(5),
	@Title NVARCHAR(30) OUTPUT
AS
BEGIN
	SELECT @Title = c.ContactTitle
	FROM Customers c
	WHERE c.CustomerID = @Id;
END;
*/
/*
DECLARE @Res NVARCHAR(30)
EXEC GetTitle 'PARIS', @Res OUTPUT;
SELECT @Res AS Title
*/

EXEC sp_help 'GetTitle'
