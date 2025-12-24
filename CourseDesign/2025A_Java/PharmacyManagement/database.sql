-- =============================
-- 1. 分类表：category
-- =============================
CREATE TABLE category (
    id INT PRIMARY KEY AUTO_INCREMENT,
    name VARCHAR(50) NOT NULL
);


-- =============================
-- 2. 供应商表：supplier
-- =============================
CREATE TABLE supplier (
    id INT PRIMARY KEY AUTO_INCREMENT,
    name VARCHAR(100) NOT NULL,
    phone VARCHAR(50),
    address VARCHAR(255)
);


-- =============================
-- 3. 用户表：user
-- =============================
CREATE TABLE user (
    id INT PRIMARY KEY AUTO_INCREMENT,
    username VARCHAR(50) UNIQUE NOT NULL,
    password VARCHAR(100) NOT NULL,   -- 存哈希
    role VARCHAR(20) NOT NULL,        -- admin / staff
    create_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);


-- =============================
-- 4. 药品表：drug
-- =============================
CREATE TABLE drug (
    id INT PRIMARY KEY AUTO_INCREMENT,
    name VARCHAR(100) NOT NULL,
    barcode VARCHAR(100) UNIQUE,
    category_id INT,
    supplier_id INT,
    price DECIMAL(10,2) NOT NULL,
    stock INT NOT NULL DEFAULT 0,
    description VARCHAR(255),
    expire_date DATE,
    create_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP,

    FOREIGN KEY (category_id) REFERENCES category(id)
      ON DELETE SET NULL
      ON UPDATE CASCADE,

    FOREIGN KEY (supplier_id) REFERENCES supplier(id)
      ON DELETE SET NULL
      ON UPDATE CASCADE
);


-- =============================
-- 5. 库存变动记录：stock_history
-- =============================
CREATE TABLE stock_history (
    id INT PRIMARY KEY AUTO_INCREMENT,
    drug_id INT NOT NULL,
    change_amount INT NOT NULL,         -- 正数=入库  负数=减少
    info VARCHAR(255),
    create_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP,

    FOREIGN KEY (drug_id) REFERENCES drug(id)
      ON DELETE CASCADE
      ON UPDATE CASCADE
);


-- =============================
-- 6. 销售记录：sales
-- =============================
CREATE TABLE sales (
    id INT PRIMARY KEY AUTO_INCREMENT,
    drug_id INT NOT NULL,
    quantity INT NOT NULL,
    total_price DECIMAL(10,2) NOT NULL,
    sale_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    user_id INT,

    FOREIGN KEY (drug_id) REFERENCES drug(id)
      ON DELETE CASCADE
      ON UPDATE CASCADE,

    FOREIGN KEY (user_id) REFERENCES user(id)
      ON DELETE SET NULL
      ON UPDATE CASCADE
);
