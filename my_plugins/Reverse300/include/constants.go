package main

import (
	"fmt"
	"io"
	"log"
	"os"
	"strings"
)

var (
	sharps = []string{"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"}
	flats  = []string{"C", "Db", "D", "Eb", "E", "F", "Gb", "G", "Ab", "A", "Bb", "B"}
)

type Note struct {
	Name  string
	Value float64
}

func main() {
	file, err := os.Create("constants.h")
	if err != nil {
		log.Fatal(err)
	}

	defer file.Close()

	render(file, getNotes())
}

func getNotes() (notes []Note) {
	for i := 0; i < 128; i++ {
		value := float64(i) / 128

		// Hex representation (e.g. "0x3e")
		notes = append(notes, Note{
			Name:  fmt.Sprintf("0x%02x", i),
			Value: value,
		})

		// Decimal representation (e.g. "61")
		notes = append(notes, Note{
			Name:  fmt.Sprintf("%d", i),
			Value: value,
		})

		// '#' representation (e.g. "C#3")
		notes = append(notes, Note{
			Name:  fmt.Sprintf("%s%d", sharps[i%12], i/12-2),
			Value: value,
		})
		notes = append(notes, Note{
			Name:  strings.ToLower(fmt.Sprintf("%s%d", sharps[i%12], i/12-2)),
			Value: value,
		})

		// 'b' representation (e.g. "Db3")
		if len(flats[i%12]) > 1 {
			notes = append(notes, Note{
				Name:  fmt.Sprintf("%s%d", flats[i%12], i/12-2),
				Value: value,
			})
			notes = append(notes, Note{
				Name:  strings.ToLower(fmt.Sprintf("%s%d", flats[i%12], i/12-2)),
				Value: value,
			})

		}
	}

	return notes
}

func render(output io.Writer, notes []Note) {
	fmt.Fprintf(output, `// Generated by 'go run constants.go' do not modify.
#pragma once

#define NoteNameLength %d

namespace Reverse300 {
const char *Notes[12] = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};

struct Note {
public:
  const char *Name;
  double Value;
};

const Note NoteName[%d] = {
`, len(notes), len(notes))

	for i, note := range notes {
		if i == len(notes)-1 {
			fmt.Fprintf(output, "{\"%v\", %f}\n", note.Name, note.Value)
		} else {
			fmt.Fprintf(output, "{\"%v\", %f}\n,", note.Name, note.Value)
		}
	}

	fmt.Fprintln(output, `};
} // namespace Reverse300`)
}
