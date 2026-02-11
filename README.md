# myFinger: User Information Utility

This university project defines a reimplemented version of the classic `finger` command in Unix/Linux systems. It retrieves and prints information about system users, including:

- Full name and GECOS fields  
- Login status and idle time  
- Last login date  
- Office and home phone numbers  
- Mail and plan status (`.pgpkey`, `.project`, `.plan` files)

> A full report of the project, written in Italian, is included in Relazione_HW1.pdf.
## Requirements

- GCC or Clang compiler  
- Linux or macOS system (with POSIX libraries)  

> The program uses system files like `/etc/passwd` and `/var/mail/` to retrieve user information.

## Compilation

Compile the program using `gcc`:

```bash
gcc myFinger.c -o myFinger
```
No external libraries are required beyond standard C headers.

## Usage

Run the program in the terminal as follows:

```bash
./myFinger [options] [username1 username2 ...]
```

## Command Options

The program supports the following **options**, which specify different **output formats**:

- `-s` : Display user data in **short format**. The output is compact and tabular.

- `-l` : Display user data in **long format**. If both `-s` and `-l` are provided, `-l` takes precedence. If neither is specified, the default behavior is long format.

- `-m` : Match input usernames **only to the system login names**, not to extended/full names (GECOS fields).

- `-p` : When using long format (`-l`), **do not display the user's plan information**. In short format (`-s`), this option is ignored.
