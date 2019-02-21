Ονοματεπωνυμο: Ορφανίδης Ελευθέριος
Α.Μ.: 1115201400133

Compile: Για να γινει compile το προγραμμα τρεξε την εντολη make .

Run: ./shapes -i inputfile -w workercount -d tempdirectory
	Οπου:
	-inputfile : το αρχειο εισοδου
	-workercount: το πληθος των workers που θα τρεξουν καθε utility
	-tempdirectory: το ονομα του προσωρινου directory στο οποιο θα αποθηκευονται τα fifos και τα 		outputs των handlers (πχ: handlerid.txt).

Εχω υλοποιησει ολα τα utilities. Το προγραμμα αρχικα παιρνει τα arguments απο το command line και στην περιπτωση του temp directory κανω ενα fork και στην child process εκτελω ενα execlp ωστε να δημιουργησω με την εντολη mkdir το καινουριο directory.Μετα υπολογιζω τον αριθμο των records που εχει το αρχειο εισοδου και φτιαχνω ενα πινακα int numtoread[workercount] (τον δεσμευω δυναμικα) ωστε να μοιραστουν οσο πιο δικαια γινεται τα records σε ολους τους workers. Αρχικα καθε worker παιρνει το (numofrecords/workercount) , μετα μοιραζω το (numofrecords mod workercount) στους workers παιρνοντας ο καθε worker το πολυ 1 record παραπανω. Μετα μπαινω στο loop του cli και για καθε γραμμη εισοδου που παιρνω , "σπαω" με την βοηθεια της strtok την γραμμη στις διαφορετικες εντολες που περιεχει και για καθε διαφορετικη εντολη κανω ενα fork() ωστε καθε command να εχει τον δικο του handler. Μεσα στον handler δημιουργω τα fifo αρχεια για καθε worker και το output του ιδιου του handler και μετα μπαινω σε ενα for loop (0 - workercount) και σε καθε επαναληψη κανω fork(). Στο child process κανω το execvp για τον συγκεκριμενο worker.
Μετα το for loop εισαγω στο fdarray[workercount] (struct pollfd) ολα τα διαφορετικα fifo αρχεια του handler και ξεκιναω να διαβαζω απο οποιο fifo file εχει δεδομενα για διαβασμα (οποιος worker τελειωσε πρωτος) χρησιμοποιωντας την συναρτηση poll.
Στο τελος του handler διαγραφω οσες μεταβλητες ειχα κανει malloc και μετα κανω exit() καθως η συγκεκριμενη διεργασια αποτελει child process και πρεπει να τερματισει ( ο master περιμενει να τελειωσουν ολοι οι handlers).
Μολις τελειωσουν ολοι οι handlers ο master εκτελει το gnuplot script ( με την χρηση fork() και exec()) και συνεχιζει το cli μεχρι να δεχτει την εντολη exit.
