package main

import (
	"bufio"
	"fmt"
	"os"
)

type Tree struct {
    height int
}

func NewTree (height int) Tree {
    return Tree{
        height: height,
    }
}

func lineProcessing(content string) []Tree {
    var trees []Tree
    for i := 0; i < len(content); i++ {
        height := content[i] - '0'
        trees = append(trees, NewTree(int(height)))
    }

    return trees
}

type MatrixTree [][]Tree
func (trees MatrixTree) scanLineVertical(begin, end int, visible []bool) {

    tallest := make([]int, 0); for i:= 0; i < len(trees[0]); i++ { tallest = append(tallest, -1) }
    for scan := begin; scan < end; scan++ {
        for x := 0; x < len(trees); x++ {
            if trees[x][scan].height > tallest[x] {
                tallest[x] = trees[x][scan].height
                visible[x*len(trees[0])+scan] = true
            }
        }
    }

    tallest = make([]int, 0); for i:= 0; i < len(trees[0]); i++ { tallest = append(tallest, -1) }
    for scan := end-1; scan >= begin; scan-- {
        for x := 0; x < len(trees); x++ {
            if trees[x][scan].height > tallest[x] {
                tallest[x] = trees[x][scan].height
                visible[x*len(trees[0])+scan] = true
            }
        }
    }

}

func (trees MatrixTree) scanLineHorizontal(begin, end int, visible []bool) {

    tallest := make([]int, 0); for i:= 0; i < len(trees[0]); i++ { tallest = append(tallest, -1) }

    for scan := begin; scan < end; scan++ {
        for x := 0; x < len(trees[0]); x++ {
            if trees[scan][x].height > tallest[x] {
                tallest[x] = trees[scan][x].height
                visible[scan*len(trees[0])+x] = true
            }
        }
    }

    tallest = make([]int, 0); for i:= 0; i < len(trees[0]); i++ { tallest = append(tallest, -1) }
    for scan := end-1; scan >= begin; scan-- {
        for x := 0; x < len(trees[0]); x++ {
            if trees[scan][x].height > tallest[x] {
                tallest[x] = trees[scan][x].height
                visible[scan*len(trees[0])+x] = true
            }
        }
    }

}

func (trees MatrixTree) scenicScore(y, x int) int {
    //up
    upVal := 0
    for i := y-1; i >= 0; i-- {
        upVal = y-i
        if trees[i][x].height >= trees[y][x].height {
            break
        }
    }
    //down
    downVal := 0
    for i:= y+1; i < len(trees); i++ {
        downVal = i-y
        if trees[i][x].height >= trees[y][x].height {
            break
        }
    }
    //left
    leftVal := 0
    for i:= x-1; i >= 0; i-- {
        leftVal = x-i
        if trees[y][i].height >= trees[y][x].height {
            break
        }
    }
    //right
    rightVal := 0
    for i := x+1; i < len(trees[0]); i++ {
        rightVal = i-x
        if trees[y][i].height >= trees[y][x].height {
            break
        }
    }

    //fmt.Printf("TOP: %d DOWN: %d LEFT: %d RIGHT %d\n", upVal, downVal, leftVal, rightVal)
    return upVal*downVal*rightVal*leftVal
}

func (trees MatrixTree) scenicScores() int {

    max := 0
    for i := 1; i < len(trees)-1; i++ {
        for j := 1; j < len(trees[0])-1; j++ {
            sceneScore := trees.scenicScore(i, j)
            //fmt.Printf("(%d, %d) = %d\n", i, j, sceneScore)
            if sceneScore > max {
                max = sceneScore
            }
        }
    }

    return max
}

func main() {
    file, _ := os.Open("input")
    defer file.Close()
    reader := bufio.NewReader(file)
    var trees MatrixTree

    for y := 0 ; ; y++ {
        content, err  := reader.ReadString('\n')
        if err != nil {
            break
        }

        content = content[:len(content)-1]
        trees = append(trees, lineProcessing(content))
    }

    var visible = make([]bool, len(trees)*len(trees[0]))
    //top and bottom scan line
    trees.scanLineHorizontal(0, len(trees), visible)
    //right and left scan line
    trees.scanLineVertical(0, len(trees[0]), visible)


    var totalVisible = 0
    for i := 0; i < len(visible); i++ {
        if(visible[i]) {
            totalVisible++
        }
    }
    fmt.Printf("\nVisible Trees: %d\n", totalVisible)

    maxScenicScore := trees.scenicScores()
    fmt.Printf("\nMax Scenic Score %d\n", maxScenicScore)
}
