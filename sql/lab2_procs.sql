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






--additional







USE NORTHWND


--1

GO
/*
CREATE PROC ProductsByCategory
    @CategoryID INT
AS
BEGIN
    SELECT ProductName, UnitPrice
    FROM Products
    WHERE CategoryID = @CategoryID
END; */


EXEC ProductsByCategory 1;

--2
GO
CREATE PROC OrdersByCountry
    @Country VARCHAR(25) = 'Germany'
AS
BEGIN
    SELECT *
    FROM Orders
    WHERE ShipCountry = @Country
END;

EXEC OrdersByCountry;

EXEC OrdersByCountry 'France';


--3

GO
CREATE PROC GetOrderCounthw
    @CustomerID NCHAR(5),
    @OrderCount INT OUTPUT
AS
BEGIN
    SELECT @OrderCount = COUNT(*)
    FROM Orders
    WHERE CustomerID = @CustomerID
END;


/* Test */
DECLARE @Count INT

EXEC GetOrderCounthw 'ALFKI', @Count OUTPUT

SELECT @Count AS OrderCount;
GO

--4

EXEC sp_helptext 'ProductsByCategory';


EXEC sp_help 'ProductsByCategory';


/*
sp_helptext:
Ցույց է տալիս պռոցեդուռայի կոդը

sp_help:
Ցույց է տալիս պռեցեդուռայի նկարագրությունը, իր պարամետրերը և մետատվյալները
*/


--5

GO
CREATE PROC GetCustomersByCountry
    @Country NVARCHAR(15) = NULL
AS
BEGIN
    IF @Country IS NULL
    BEGIN
        PRINT N'Խնդրում ենք նշել երկիրը'
        RETURN
    END

    SELECT CustomerID, CompanyName
    FROM Customers
    WHERE Country = @Country
END;


EXEC GetCustomersByCountry;


EXEC GetCustomersByCountry 'Germany';

--6

GO
CREATE TABLE OrderNotes
(
    NoteID INT IDENTITY,
    NoteText VARCHAR(50),
    CreatedAt DATETIME
);



GO
CREATE PROC AddTestNotes
    @rows_count INT
AS
BEGIN
    DECLARE @i INT
    SET @i = 1

    WHILE @i <= @rows_count
    BEGIN
        INSERT INTO OrderNotes (NoteText, CreatedAt)
        VALUES ('Note #' + CAST(@i AS VARCHAR(10)), GETDATE())

        SET @i = @i + 1
    END
END;



EXEC AddTestNotes 5;



SELECT *
FROM OrderNotes;


--7

GO
CREATE PROC GetProductPriceSafe
    @ProductID INT = NULL,
    @Price MONEY OUTPUT
AS
BEGIN
    IF @ProductID IS NULL
    BEGIN
        PRINT N'Խնդրում ենք նշել ապրանքի ID-ն'
        RETURN
    END

    IF NOT EXISTS
    (
        SELECT *
        FROM Products
        WHERE ProductID = @ProductID
    )
    BEGIN
        PRINT N'Այդպիսի ապրանք գոյություն չունի'
        RETURN
    END

    SELECT @Price = UnitPrice
    FROM Products
    WHERE ProductID = @ProductID
END;

-- no id

DECLARE @Price MONEY

EXEC GetProductPriceSafe @Price = @Price OUTPUT;


-- nonexisting id

SET @Price = NULL

EXEC GetProductPriceSafe 99999, @Price OUTPUT;

-- normal execution

SET @Price = NULL

EXEC GetProductPriceSafe 1, @Price OUTPUT

SELECT @Price AS ProductPrice;
