package main

import (
	"fmt"
	"log"
	"os"
	"strconv"
)

type operator int

var logger *log.Logger

const (
	INT = "INT"
	STR = "STRING"
)

type Ast struct {
	kind  string
	ival  int
	sval  string
	left  *Ast
	right *Ast
}

func astOp(op string, left *Ast, right *Ast) *Ast {
	r := new(Ast)
	r.kind = op
	r.left = left
	r.right = right
	return r
}

func astInt(val int) *Ast {
	r := new(Ast)
	r.kind = INT
	r.ival = val
	return r
}

func astStr(str string) *Ast {
	r := new(Ast)
	r.kind = STR
	r.sval = str
	return r
}

// ToDo: must print all args
func printError(err ...string) {
	if len(err) == 1 {
		fmt.Print(err)
		os.Exit(1)
	}
	eMsg := fmt.Sprintf(err[0], err[1])
	fmt.Print(eMsg)
	os.Exit(1)
}

func (ast Ast) printInt() {
	fmt.Sprint(`.text\n\t
.global intfn\n
intfn:\n\t`)
	fmt.Sprintf("mov $%d, %%eax\n\t", ast.ival)
	fmt.Sprint("ret\n")
}

func (ast Ast) printString() {

}

func (ast Ast) print() {
	if ast.kind == INT {
		ast.printInt()
	} else {
		ast.printString()
	}
}

func readNumber(expr string) *Ast {
	logger.Printf("expr: %v", expr)
	var num int
	for _, r := range expr {
		logger.Printf("r: %v", string(r))
		logger.Printf("num: %v", num)
		n, err := strconv.Atoi(string(r))
		if err != nil {
			return astInt(num)
		}
		num = num*10 + n
	}
	return nil
}

func readString(expr string) *Ast {
	return astStr(expr)
}

func readPrim(expr string) *Ast {
	firstChar := string(expr[0])
	logger.Printf("firstChar: %v", firstChar)

	_, err := strconv.Atoi(firstChar)
	if err == nil {
		return readNumber(expr)
	} else if firstChar == "\"" {
		return readString(expr)
	}
	printError("Can't handle '%v'", firstChar)
	return nil
}

func priority(op string) int {
	logger.Printf("op: %v", op)
	switch op {
	case "+":
	case "-":
		return 1
	case "*":
	case "/":
		return 2
	default:
		printError("Unknown binary operator: %v\n", op)
		return 0
	}
	return 0
}

func newAst(prec int) *Ast {
	expr := os.Args[1]
	ast := readPrim(expr)
	logger.Printf("ast.kind: %v", ast.kind)
	logger.Printf("ast.ival: %v", ast.ival)
	for _, r := range expr {
		logger.Printf("r: %v", string(r))
		if string(r) == " " {
			continue
		}
		prec2 := priority(string(r))
		if prec2 < prec {
			return ast
		}
		ast = astOp(string(r), ast, newAst(prec2+1))
	}
	return ast
}

func main() {
	logger = log.New(os.Stdout, "logger: ", log.Lshortfile)
	logger.Print("start")
	ast := newAst(0)
	ast.print()
	os.Exit(0)
}
