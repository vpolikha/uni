--

-- 1
USE NORTHWND;
GO

BEGIN TRANSACTION;

UPDATE Orders
SET Freight = 1.0
WHERE Freight > 0.75
  AND Freight < 1.0;

IF @@ROWCOUNT > 0
    BEGIN
        COMMIT TRANSACTION;
    END
ELSE
    BEGIN
        ROLLBACK TRANSACTION;
        PRINT 'No such data';
    END;
GO


-- 2
USE NORTHWND;
GO

BEGIN TRANSACTION;

DELETE FROM [Order Details]
WHERE ProductID IN
      (
          SELECT ProductID
          FROM Products
          WHERE UnitsInStock = 0
      );

DELETE FROM Products
WHERE UnitsInStock = 0;

SELECT *
FROM Products
WHERE UnitsInStock = 0;

ROLLBACK TRANSACTION;

SELECT *
FROM Products
WHERE UnitsInStock = 0;
GO



--3

BEGIN TRANSACTION;


SAVE TRANSACTION sp2;
UPDATE [Order Details]
SET Discount = Discount + 0.5
WHERE Quantity BETWEEN 10 AND 15;

SELECT Quantity, Discount FROM [Order Details];

ROLLBACK TRANSACTION sp2;

SELECT Quantity, Discount FROM [Order Details];

ROLLBACK TRANSACTION;
GO

-- tg
CREATE OR ALTER TRIGGER trg_checkdiscount
ON [Order Details]
INSTEAD OF UPDATE
AS
BEGIN
    SET NOCOUNT ON;

    IF EXISTS (SELECT 1 FROM inserted WHERE Discount > 0.5)
        PRINT 'Cannot be more than 05';
    ELSE
        UPDATE od
        SET od.UnitPrice = i.UnitPrice,
            od.Quantity  = i.Quantity,
            od.Discount  = i.Discount
        FROM [Order Details] od
        JOIN inserted i
          ON od.OrderID = i.OrderID
         AND od.ProductID = i.ProductID;
END;
GO

-- upd
BEGIN TRANSACTION;

UPDATE [Order Details]
SET Discount = 0.4
WHERE Quantity = 100;

SELECT Quantity, Discount FROM [Order Details] WHERE Quantity = 100;
SELECT @@TRANCOUNT AS OpenTrans;   -- 1

ROLLBACK TRANSACTION;
GO




BEGIN TRANSACTION;

UPDATE [Order Details]
SET Discount = 0.6
WHERE Quantity = 100;

SELECT Quantity, Discount FROM [Order Details] WHERE Quantity = 100;
SELECT @@TRANCOUNT AS OpenTrans;   -- 1

ROLLBACK TRANSACTION;
GO




--additional
USE NORTHWND
GO
CREATE TRIGGER trg_OrderTotalDiscount
ON [Order Details]
AFTER INSERT
AS
BEGIN
    SET NOCOUNT ON;

    BEGIN TRY
        BEGIN TRANSACTION;

        UPDATE od
        SET od.Discount = 0.10
        FROM [Order Details] od
        WHERE od.OrderID IN (SELECT OrderID FROM inserted)
        AND (SELECT SUM(x.UnitPrice * x.Quantity)
               FROM [Order Details] x
               WHERE x.OrderID = od.OrderID) > 1000;

        COMMIT TRANSACTION;
    END TRY
    BEGIN CATCH
        ROLLBACK TRANSACTION;
        PRINT 'No Discount';
    END CATCH;
END;
GO

USE NORTHWND
GO
CREATE TRIGGER trg_GermanyDiscount
ON [Order Details]
AFTER INSERT
AS
BEGIN
    SET NOCOUNT ON;
    BEGIN TRANSACTION;

    UPDATE od
    SET od.Discount = 0.05
    FROM [Order Details] od
    JOIN Orders o ON o.OrderID = od.OrderID
    WHERE od.OrderID IN (SELECT OrderID FROM inserted) AND o.ShipCountry = 'Germany';
    COMMIT TRANSACTION;
END;
GO







BEGIN TRANSACTION;
INSERT INTO Orders (CustomerID, EmployeeID, OrderDate, ShipCountry)
VALUES ('ALFKI', 1, GETDATE(), 'France');
DECLARE @id int = SCOPE_IDENTITY();

INSERT INTO [Order Details] (OrderID, ProductID, UnitPrice, Quantity, Discount)
VALUES (@id, 1, 18, 40, 0), (@id, 2, 19, 30, 0);

SELECT * FROM [Order Details] WHERE OrderID = @id;
ROLLBACK TRANSACTION;
GO


USE NORTHWND
BEGIN TRANSACTION;
INSERT INTO Orders(CustomerID, EmployeeID, OrderDate, ShipCountry)
VALUES ('ALFKI', 1, GETDATE(), 'France');
DECLARE @id int = SCOPE_IDENTITY();

INSERT INTO [Order Details] (ProductID, UnitPrice, Quantity, Discount)
VALUES (1, 18, 10, 0);

SELECT * FROM [Order Details] WHERE OrderID = @id;
ROLLBACK TRANSACTION;
GO




BEGIN TRANSACTION;
INSERT INTO Orders (CustomerID, EmployeeID, OrderDate, ShipCountry)
VALUES ('ALFKI', 1, GETDATE(), 'Germany');
DECLARE @id int = SCOPE_IDENTITY();

INSERT INTO [Order Details] (OrderID, ProductID, UnitPrice, Quantity, Discount)
VALUES (@id, 1, 18, 10, 0);

SELECT * FROM [Order Details] WHERE OrderID = @id;
ROLLBACK TRANSACTION;
GO













USE NORTHWND
GO
CREATE OR ALTER TRIGGER trg_ORINS
ON [ORDER DETAILS]
AFTER INSERT
AS
BEGIN
BEGIN TRANSACTION
    UPDATE od
    SET Discount += 0.05
    FROM [Order Details] od
    JOIN inserted i ON od.OrderID = i.OrderID
    JOIN Orders o ON o.OrderID = i.OrderID
    WHERE o.ShipCountry = 'Germany' 

    UPDATE od
    SET Discount += 0.1
    FROM [Order Details] od
    JOIN inserted i ON od.OrderID = i.OrderID
    JOIN Orders o ON o.OrderID = i.OrderID
    Where( SELECT SUM(UnitPrice * Quantity)
    FROM [Order Details]
    WHERE OrderID = i.OrderID ) > 1000

IF @@ERROR <> 0
BEGIN
    ROLLBACK TRANSACTION
    PRINT N'Սխալ է առաջացել'
END
ELSE
BEGIN
    COMMIT TRANSACTION
END

END
GO

INSERT INTO Orders (CustomerID, EmployeeID, OrderDate, RequiredDate, ShipCountry)
VALUES ('ALFKI', 1, GETDATE(), DATEADD(DAY,7,GETDATE()), 'Germany' )

DECLARE @NEWORDERID INT = SCOPE_IDENTITY();
INSERT INTO [Order Details] (OrderID, ProductID, UnitPrice, Quantity, Discount)
VALUES (@NEWORDERID, 1, 250, 5, 0.01 );

select * from  [Order Details]  where ORDERID = @NEWORDERID
