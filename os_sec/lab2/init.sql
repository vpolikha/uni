-- Create a simple table and seed a row.
DROP TABLE IF EXISTS accounts;
CREATE TABLE accounts (
  id INT PRIMARY KEY,
  balance INT NOT NULL
) ENGINE=InnoDB;

INSERT INTO accounts (id, balance) VALUES (1, 100);

-- Show current isolation (default: REPEATABLE READ here).
SELECT @@transaction_isolation AS default_isolation;
