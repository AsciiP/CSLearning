public class ConsumptionSystem {

    public static void main(String[] args) throws InterruptedException {
        final int PRODUCERS = 2;
        final int CONSUMERS = 3;
        final int EACH_PRODUCE_MAX = 10;
        final int TOTAL_TO_PRODUCE = PRODUCERS * EACH_PRODUCE_MAX;

        Store store = new Store(TOTAL_TO_PRODUCE);

        Thread[] producers = new Thread[PRODUCERS];
        Thread[] consumers = new Thread[CONSUMERS];

        for (int i = 0; i < PRODUCERS; i++) {
            final int id = i + 1;
            producers[i] = new Thread(new Producer(id, EACH_PRODUCE_MAX, store), "生产者-" + id);
            producers[i].start();
        }

        for (int i = 0; i < CONSUMERS; i++) {
            final int id = i + 1;
            consumers[i] = new Thread(new Consumer(id, store), "消费者-" + id);
            consumers[i].start();
        }

        // wait for producers to finish
        for (Thread p : producers) {
            p.join();
        }
        // producers finished; notify consumers in case they're waiting and no more will arrive
        synchronized (store) {
            store.notifyAllWaiting();
        }

        // wait for consumers to finish
        for (Thread c : consumers) {
            c.join();
        }

        System.out.println("所有的生产者和消费者已完成。");
    }

    // shared store
    static class Store {
        private int productCount = 0;
        private int producedSoFar = 0;
        private final int totalToProduce;

        public Store(int totalToProduce) {
            this.totalToProduce = totalToProduce;
        }

        // called by producers to produce one item
        public synchronized void produce(int producerId) throws InterruptedException {
            // produce one item
            productCount++;
            producedSoFar++;
            System.out.printf("生产者-%d 生产了1个产品。已生产=%d，库存=%d%n",
                    producerId, producedSoFar, productCount);
            // wake up consumers
            notifyAll();
        }

        // called by consumers to consume one item
        // returns true if consumed an item, false if no more items will ever be available
        public synchronized boolean consume(int consumerId) throws InterruptedException {
            // wait while no items but producers may still produce
            while (productCount == 0) {
                if (producedSoFar >= totalToProduce) {
                    // no more will be produced and store empty -> finish
                    return false;
                }
                wait();
            }
            // consume one
            productCount--;
            System.out.printf("消费者-%d 消费了1个产品。库存=%d%n", consumerId, productCount);
            // optionally wake producers (not needed here since no capacity limit) or other consumers
            notifyAll();
            return true;
        }

        // helper to wake any waiting threads (used after producers finish)
        public synchronized void notifyAllWaiting() {
            notifyAll();
        }
    }

    static class Producer implements Runnable {
        private final int id;
        private final int maxProduce;
        private final Store store;

        public Producer(int id, int maxProduce, Store store) {
            this.id = id;
            this.maxProduce = maxProduce;
            this.store = store;
        }

        @Override
        public void run() {
            for (int i = 0; i < maxProduce; i++) {
                try {
                    store.produce(id);
                    // simulate production time
                    java.util.concurrent.TimeUnit.MILLISECONDS.sleep((long) (Math.random() * 200) + 50);
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                    break;
                }
            }
            System.out.printf("生产者-%d 完成生产。%n", id);
        }
    }

    static class Consumer implements Runnable {
        private final int id;
        private final Store store;

        public Consumer(int id, Store store) {
            this.id = id;
            this.store = store;
        }

        @Override
        public void run() {
            try {
                while (true) {
                    boolean consumed = store.consume(id);
                    if (!consumed) {
                        break; // no more items will come 
                    }
                    // simulate consumption time
                    java.util.concurrent.TimeUnit.MILLISECONDS.sleep((long) (Math.random() * 300) + 50);
                }
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }
            System.out.printf("消费者-%d 完成消费。%n", id);
        }
    }
}