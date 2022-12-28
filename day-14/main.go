package main

import (
    _ "embed"
	"fmt"
	"os"
	"strings"
    "strconv"
)

var logFile *os.File = nil

//go:embed input.txt
var input string

type Point struct {
    x int
    y int
}

type Sand struct {
    position Point
    amount int
}

func NewSand() Sand {
    return Sand {
        position: Point{ x: 500, y: 0},
        amount: 0,
    }
}

func (self *Sand) resetPosition() {
    self.position = Point{x: 500, y:0}
    self.amount += 1
}

func make_sure_existence(_grid *map[int]map[int]bool, x int) {
    grid := *_grid
    if grid[x] == nil {
        grid[x] = make(map[int]bool)
    }
}

func (self *Sand) move(_grid *map[int]map[int]bool) bool {
    grid := *_grid

    make_sure_existence(_grid, self.position.x)

    if grid[self.position.x][self.position.y+1] { //sand over floor

        make_sure_existence(_grid, self.position.x+1)
        make_sure_existence(_grid, self.position.x-1)

        if grid[self.position.x-1][self.position.y+1] == false { //sand to the left
            self.position.y += 1
            self.position.x -= 1
            //fmt.Fprintln(logFile, "sand to the left [", self.position.x, ", ", self.position.y, "]")
            //fmt.Scanln()
            return false
        } else if grid[self.position.x+1][self.position.y+1] == false { //sand to the right
            self.position.y += 1
            self.position.x += 1
            //fmt.Fprintln(logFile, "sand to the right [", self.position.x, ", ", self.position.y, "]")
            //fmt.Scanln()
            return false
        } else {
            //fmt.Fprintln(logFile, "sand stuck [", self.position.x, ", ", self.position.y, "]")
            //fmt.Scanln()
            grid[self.position.x][self.position.y] = true
            self.resetPosition()
            return false
        }

    } else {
        found := -1
        for k := range grid[self.position.x] {
            if k > self.position.y && grid[self.position.x][k] {
                if grid[self.position.x][k-1] == false {
                    if found == -1 {
                        found = k-1
                    } else {
                        found = min(found, k-1)
                    }
                }
            }
        }
        if found != -1 {
            //fmt.Fprintf(logFile, "sand fell from [ %d , %d ] to ", self.position.x,  self.position.y)
            self.position.y = found
            //fmt.Fprintf(logFile, "[ %d , %d ]\n", self.position.x,  self.position.y)
            //fmt.Scanln()
        }else {
            //fmt.Fprintln(logFile, "send fell to infinity, it was in [", self.position.x, ", ", self.position.y, "]")
            //fmt.Scanln()
            return true
        }
    }
    return false
}

func max(l int, r int) int {
    if l > r {
        return l
    }
    return r
}

func min(l int, r int) int {
    if l < r {
        return l
    }
    return r
}

func SandComing() Point {
    return Point {
        x: 500,
        y: 0,
    }
}

func init() {
    input = strings.ReplaceAll(input, "\r", "")
    input = strings.TrimRight(input, "\n")
}

func parseInput() (grid map[int]map[int]bool, lowest int) {
    grid = make(map[int]map[int]bool)
    lowest = -1
    var point *Point = nil

    for _, line := range strings.Split(input, "\n") {
        coordinates := strings.Split(line, " -> ")
        for _, coord := range coordinates {
            values := strings.Split(coord, ",")
            x, _ := strconv.Atoi(values[0])
            y, _ := strconv.Atoi(values[1])

            lowest = max(lowest, y)

            if point != nil {
                if point.x == x {
                    for i := min(point.y, y) ; i <= max(point.y, y) ; i++ {
                        make_sure_existence(&grid, x)
                        grid[x][i] = true
                    }
                }else {
                    for i := min(point.x, x) ; i <= max(point.x, x) ; i++ {
                        make_sure_existence(&grid, i)
                        grid[i][y] = true
                    }
                }
            } else {
                point = new(Point)
            }

            point.x = x
            point.y = y
        }
        point = nil
    }

    return
}

func part1(grid map[int]map[int]bool) (result int) {
    hasFallenInVoid := false
    var sand Sand = NewSand()

    for !hasFallenInVoid {
        hasFallenInVoid = sand.move(&grid)
    }
    fmt.Println(sand.amount)
    result = sand.amount
    return
}

func (self *Sand) infiniteMove(_grid *map[int]map[int]bool, bottom int) {
    grid := *_grid

    make_sure_existence(_grid, self.position.x)

    if grid[self.position.x][self.position.y+1] { //sand over floor

        make_sure_existence(_grid, self.position.x+1)
        make_sure_existence(_grid, self.position.x-1)

        if grid[self.position.x-1][self.position.y+1] == false { //sand to the left
            self.position.y += 1
            self.position.x -= 1
            fmt.Fprintln(logFile, "sand to the left [", self.position.x, ", ", self.position.y, "]")
            //fmt.Scanln()
            return
        } else if grid[self.position.x+1][self.position.y+1] == false { //sand to the right
            self.position.y += 1
            self.position.x += 1
            fmt.Fprintln(logFile, "sand to the right [", self.position.x, ", ", self.position.y, "]")
            //fmt.Scanln()
            return
        } else {
            fmt.Fprintln(logFile, "sand stuck [", self.position.x, ", ", self.position.y, "]")
            //fmt.Scanln()
            grid[self.position.x][self.position.y] = true
            self.resetPosition()
            return
        }

    } else {
        found := -1
        for k := range grid[self.position.x] {
            if k > self.position.y && grid[self.position.x][k] {
                if grid[self.position.x][k-1] == false {
                    if found == -1 {
                        found = k-1
                    } else {
                        found = min(found, k-1)
                    }
                }
            }
        }
        if found != -1 {
            fmt.Fprintf(logFile, "sand fell from [ %d , %d ] to ", self.position.x,  self.position.y)
            self.position.y = found
            fmt.Fprintf(logFile, "[ %d , %d ]\n", self.position.x,  self.position.y)
            //fmt.Scanln()
        }else {
            fmt.Fprint(logFile, "send fell to the bottom and got stuck, it was in [", self.position.x, ", ", self.position.y, "]")
            self.position.y = bottom-1
            fmt.Fprintln(logFile, "and went to [", self.position.x, ", ", self.position.y, "]")
            grid[self.position.x][self.position.y] = true
            self.resetPosition()
            return
        }
    }
    return
}

func printGrid(grid map[int]map[int]bool) {

    valuesX := [2]int{ -1, -1 } //min, max
    valuesY := [2]int{ -1, -1 } //min, max

    for x, row := range grid {
        if valuesX[0] == -1 { valuesX[0] = x }

        valuesX[0] = min(valuesX[0], x)
        valuesX[1] = max(valuesX[1], x)

        for y := range row {
            if valuesY[0] == -1 { valuesY[0] = y }

            valuesY[0] = min(valuesY[0], y)
            valuesY[1] = max(valuesY[1], y)
        }

    }

    draw := ""
    for j := valuesY[0]; j <= valuesY[1]; j++ {
        for i := valuesX[0]; i <= valuesX[1]; i++ {
            make_sure_existence(&grid, i)

            if grid[i][j] {
                draw += "#"
            } else {
                draw += "."
            }
        }
        draw += "\n"
    }

    fmt.Print(draw)

}

func part2(grid map[int]map[int]bool, bottom int, initial int) {
    var sand Sand = NewSand()
    if grid[500] == nil { grid[500] = make(map[int]bool) }

    //printGrid(grid)
    for !grid[500][0] {
        sand.infiniteMove(&grid, bottom)
    }
    //printGrid(grid)
    fmt.Println(sand.amount + initial)
}

func main() {
    _logFile, _ := os.Create("log")
    logFile = _logFile

    grid, lowest := parseInput()

    result := part1(grid)
    part2(grid, lowest+2, result)
}
