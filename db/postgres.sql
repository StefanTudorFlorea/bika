CREATE TABLE Demo(
    id SERIAL PRIMARY KEY, 
    name TEXT,
    age INT
);

INSERT INTO Demo(name, age) VALUES('user-1', 42), ('user-2', 22), ('user-3', 55);