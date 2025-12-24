import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

plt.rcParams['font.sans-serif'] = ['Microsoft YaHei']

# Dataset importing and overview
df = pd.read_csv("./boston.csv")
print('=== 生数据 ===')
print(df.head())
print('=== 详细信息 ===')
print(df.describe())

# Split dataset into X and y
X = df.drop('MEDV', axis=1).values
y = df['MEDV'].values.reshape(-1, 1)
print(f'X shape: {X.shape}, y shape: {y.shape}')

# Normalization
X_mean = X.mean(axis=0)
X_std = X.std(axis=0)
X_norm = (X - X_mean) / X_std

# Add bias term (intercept) to X
X_norm = np.c_[np.ones((X_norm.shape[0], 1)), X_norm]

# Estimate Weight matrix, calc on LINEAR REGRESSION
def train_linear_regression(X, y, loss_rate=0.01, epochs=2000, step=100):
    '''
    In which:
        y_predicted = XW + b,
        n = quantity of samples (row qt.)
        d = dimension of samples (colum qt.)
    Loss f: J(W) = SUM(y_actual[i] - y_predicted[i])^2 / n
    Partial J/W = (2/n) * X.trans * (XW - y_actual)
    '''
    n, d = X.shape
    W = np.random.randn(d, 1) * 0.01
    
    for epoch in range(epochs):
        y_predicted = X @ W
        error = y_predicted - y
        loss = (error**2).mean()
        grad = (2/n) * (X.T @ error)
        W -= loss_rate * grad

        if epoch % step == 0:
            print(f'Epoch {epoch}, Loss: {loss:.4f}')

    return W

# Train the model
W = train_linear_regression(X_norm, y)
print('\n=== 训练权重 ===')
print(f'偏置 (W[0]): {W[0][0]:.4f}')
print('权重:', W[1:].flatten())

# Make predictions
y_pred = X_norm @ W

# Calculate evaluation metrics
mse = ((y - y_pred) ** 2).mean()
r2 = 1 - (np.sum((y - y_pred) ** 2) / np.sum((y - y.mean()) ** 2))

print('\n=== 模型性能 ===')
print(f'MSE: {mse:.4f}')
print(f'R²: {r2:.4f}')

# Plot 1: Actual vs Predicted on line plot
plt.figure(figsize=(10, 6))
plt.plot(y, label='实际值', color='blue', linewidth=1)
plt.plot(y_pred, label='预测值', color='orange', linewidth=2)
plt.xlabel('样本索引', fontsize=12)
plt.ylabel('房价', fontsize=12)
plt.title('实际值 vs 预测值', fontsize=14)
plt.legend()
plt.grid(True, alpha=0.3)
plt.show()

# Plot 2.1: Actual vs Predicted on scatter plot
plt.figure(figsize=(12, 5))

plt.subplot(1, 2, 1)
plt.scatter(y, y_pred, alpha=0.6, edgecolors='black', linewidths=0.5)
plt.xlabel('实际值', fontsize=12)
plt.ylabel('预测值', fontsize=12)
plt.title(f'实际值 vs 预测值 (R²={r2:.4f})', fontsize=14)
plt.plot([y.min(), y.max()], [y.min(), y.max()], 'r--', lw=2, label='完美拟合')
plt.legend()
plt.grid(True, alpha=0.3)

# Plot 2.2: Residuals
plt.subplot(1, 2, 2)
residuals = y - y_pred
plt.scatter(y_pred, residuals, alpha=0.6, edgecolors='black', linewidths=0.5)
plt.xlabel('预测值', fontsize=12)
plt.ylabel('残差', fontsize=12)
plt.title('残差图', fontsize=14)
plt.axhline(y=0, color='r', linestyle='--', lw=2, label='零残差线')
plt.legend()
plt.grid(True, alpha=0.3)

plt.tight_layout()
plt.show()