package main

import (
	"bufio"
	"fmt"
	"os"
	"strings"
    "strconv"
)

type Point struct {
    x int
    y int
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

func main() {
    file, _ := os.Open("test")
    defer file.Close()
    reader := bufio.NewReader(file)
    grid := make(map[int]map[int]bool)


    var point *Point = nil

    for {
        line, err := reader.ReadString('\n')
        if err != nil {
            break
        }
        coordinates := strings.Split(line, " -> ")
        for _, coord := range coordinates {
            trimmed := strings.Trim(coord, " \n")
            values := strings.Split(trimmed, ",")
            x, _ := strconv.Atoi(values[0])
            y, _ := strconv.Atoi(values[1])
            if point != nil {
                if point.x == x {
                    for i := min(point.y, y) ; i <= max(point.y, y) ; i++ {
                        if grid[x] == nil { grid[x] = make(map[int]bool) }
                        grid[x][i] = true
                    }
                }else {
                    for i := min(point.x, x) ; i <= max(point.x, x) ; i++ {
                        if grid[i] == nil { grid[i] = make(map[int]bool) }
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
    fmt.Println(grid)
    fmt.Println(point)
}
