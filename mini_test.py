import subprocess


def run_tests(inputs):

    inputs = ["pipes", "cd /", "pwd", "pipes", "exit"]

    with subprocess.Popen(
        ['./minishell'],
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True,
        bufsize=1,
        universal_newlines=True
                        ) as process:

        for input_str in inputs:
            process.stdin.write(input_str + '\n')
            process.stdin.flush()

        process.stdin.close()
        output = process.stdout.read()
        error = process.stderr.read()

        with open("output_log.txt", "w") as log:
            log.write(output.strip())
        # print(f"Output:\n{output.strip()}")
        with open("error_log.txt", "w") as erlog:
            erlog.write(error.strip())
        # print(f"Errors:\n{error.strip()}")


def main():

    inputs = ["pipes", "cd /", "pwd", "pipes", "exit"]
    run_tests(inputs)


if __name__ == '__main__':
    main()
