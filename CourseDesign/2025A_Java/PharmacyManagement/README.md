# 药房管理系统

## 项目简介

药房管理系统是一个基于 Java Swing 和 MySQL 开发的桌面应用程序，用于管理药房的药品、分类、供应商、库存和销售业务。系统提供了完整的用户认证、权限管理、药品管理、库存管理等功能，适用于中小型药房的日常运营。

## 项目结构

```
PharmacyManagement/
├── src/
│   ├── Main.java                    # 主程序入口
│   ├── dao/                         # 数据访问层
│   │   ├── CategoryDAO.java         # 分类管理
│   │   ├── DrugDAO.java             # 药品管理
│   │   ├── SalesDAO.java            # 销售记录管理
│   │   ├── StockHistoryDAO.java     # 库存变动记录管理
│   │   └── SupplierDAO.java         # 供应商管理
│   ├── gui/                         # GUI 界面层
│   │   ├── LoginFrame.java          # 登录界面
│   │   ├── RegisterFrame.java       # 注册界面
│   │   ├── AdminFrame.java          # 管理员主界面
│   │   ├── UserFrame.java           # 普通员工界面
│   │   ├── DrugManagementFrame.java # 药品管理
│   │   ├── StockManagementFrame.java # 库存管理
│   │   ├── ReportFrame.java         # 报表统计
│   │   ├── AddDrugDialog.java       # 添加药品对话框
│   │   ├── EditDrugDialog.java      # 编辑药品对话框
│   │   ├── CategoryDialog.java      # 分类管理对话框
│   │   ├── SupplierDialog.java      # 供应商管理对话框
│   │   └── StockOperationDialog.java # 库存操作对话框
│   ├── model/                       # 数据模型层
│   │   ├── Category.java            # 分类模型
│   │   ├── Drug.java                # 药品模型
│   │   ├── Sales.java               # 销售记录模型
│   │   ├── StockHistory.java        # 库存变动记录模型
│   │   └── Supplier.java            # 供应商模型
│   └── util/                        # 工具类
│       ├── DBUtil.java              # 数据库连接工具
│       └── PasswordUtil.java        # 密码加密工具
├── lib/                             # 依赖库（MySQL JDBC 驱动）
├── database.sql                     # 数据库初始化脚本
└── README.md                        # 项目文档
```

## 主要功能

### 1. 用户管理
- ✅ 用户注册（普通员工、管理员）
- ✅ 用户登录（密码加密存储）
- ✅ 身份认证与权限验证
- ✅ 不同角色的功能权限控制

### 2. 药品管理
- ✅ 添加、编辑、删除药品
- ✅ 药品条形码管理
- ✅ 药品分类管理
- ✅ 药品供应商关联
- ✅ 药品过期日期管理
- ✅ 药品详细信息维护

### 3. 库存管理
- ✅ 药品入库、出库操作
- ✅ 库存变动记录查询
- ✅ 库存预警功能
- ✅ 库存历史追踪

### 4. 销售管理
- ✅ 销售记录创建与查询
- ✅ 销售统计分析
- ✅ 药品销售排行
- ✅ 时间段销售数据统计

### 5. 供应商管理
- ✅ 供应商信息维护
- ✅ 供应商分类管理

## 数据库设计

### 核心表结构

#### 1. 用户表 (user)
```sql
user (id, username, password, role, create_time)
```

#### 2. 分类表 (category)
```sql
category (id, name)
```

#### 3. 供应商表 (supplier)
```sql
supplier (id, name, phone, address)
```

#### 4. 药品表 (drug)
```sql
drug (id, name, barcode, category_id, supplier_id, price, stock, description, expire_date, create_time)
```

#### 5. 库存变动记录表 (stock_history)
```sql
stock_history (id, drug_id, change_amount, info, create_time)
```

#### 6. 销售记录表 (sales)
```sql
sales (id, drug_id, quantity, sale_price, total_price, sale_time)
```

## 快速开始

### 环境要求
- JDK 17 或更高版本
- MySQL 8.0 或更高版本
- IDE：IntelliJ IDEA、Eclipse 或 NetBeans

### 安装步骤

1. **克隆项目**
```bash
git clone https://github.com/AsciiP/PharmacyManagement
cd PharmacyManagement
```

2. **配置数据库**
   - 启动 MySQL 服务
   - 创建数据库（可选，脚本会自动创建）
   - 导入数据库脚本：
   ```bash
   mysql -u root -p < database.sql
   ```

3. **配置数据库连接**
   - 编辑 `src/util/DBUtil.java` 文件
   - 修改以下配置：
   ```java
   private static final String USER = "root";
   private static final String PASSWORD = "你的MySQL密码";
   ```

4. **编译和运行**
   - 使用 IDE 打开项目
   - 编译所有 Java 文件
   - 运行 `src/Main.java` 作为主程序入口

## 开发者信息

- **开发时间**：2025-12-10
- **学科**：计算机科学与技术
- **课程**：Java语言程序设计II
- **开发者**：2405111B08 王子睿
