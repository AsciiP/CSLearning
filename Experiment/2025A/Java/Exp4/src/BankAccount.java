public class BankAccount {
    private double balance;

    public BankAccount(double initialBalance) {
        this.balance = initialBalance;
    }

    public double getBalance() {
        return balance;
    }

    public void deposit(double amount) {
        if (amount > 0) {
            balance += amount;
        }
    }

    public boolean withdraw(double amount) {
        if (amount > 0 && amount <= balance) {
            balance -= amount;
            return true;
        }
        return false;
    }
    public static void main(String[] args) {
        BankAccount account = new BankAccount(1000);
        System.out.println("Initial Balance: " + account.getBalance());
        account.deposit(500);
        System.out.println("After Deposit: " + account.getBalance());
        if (account.withdraw(200)) {
            System.out.println("After Withdrawal: " + account.getBalance());
        } else {
            System.out.println("Withdrawal Failed");
        }
    }
}
