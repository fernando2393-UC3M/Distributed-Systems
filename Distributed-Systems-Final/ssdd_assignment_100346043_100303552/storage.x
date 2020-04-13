program STORAGE {
    version STORAGEVER {
        void initializeStorage() = 1;
        string recoverTuple(string topic) = 2;
        int addTuple(string topic, string text) = 3;
    } = 1;
} = 99;