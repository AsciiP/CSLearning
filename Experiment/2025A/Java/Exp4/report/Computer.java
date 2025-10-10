public class Computer {
    private final CPU cpu;
    private final Memory memory;
    private final Motherboard motherboard;
    private final Monitor monitor;

    public Computer(CPU cpu, Memory memory, Motherboard motherboard, Monitor monitor) {
        this.cpu = cpu;
        this.memory = memory;
        this.motherboard = motherboard;
        this.monitor = monitor;
    }

    public void start() {
        motherboard.powerOn();
        cpu.process();
        memory.load();
        monitor.display("Computer is starting...");
    }

    public void runProgram(String program) {
        cpu.process();
        memory.load();
        monitor.display("Running program: " + program);
    }

    public void shutdown() {
        monitor.display("Computer is shutting down...");
        motherboard.powerOff();
    }

    public static void main(String[] args) {
        CPU cpu = new CPU();
        Memory memory = new Memory();
        Motherboard motherboard = new Motherboard();
        Monitor monitor = new Monitor();

        Computer computer = new Computer(cpu, memory, motherboard, monitor);
        computer.start();
        computer.runProgram("Example Program");
        computer.shutdown();
    }
}

class CPU {
    public void process() {
        System.out.println("CPU is processing data.");
    }
}

class Memory {
    public void load() {
        System.out.println("Memory is loading data.");
    }
}

class Motherboard {
    public void powerOn() {
        System.out.println("Motherboard is powered on.");
    }

    public void powerOff() {
        System.out.println("Motherboard is powered off.");
    }
}

class Monitor {
    public void display(String message) {
        System.out.println("Monitor: " + message);
    }
}