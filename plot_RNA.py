import ViennaRNA
seq = "GAAAACTTTTGAATATAGGGGAAAAAAAGTTTTCCAAAGACCAAAAATGGTTTTGCAAAAAAAGAAAGTTTAG"
structure = ViennaRNA.fold(seq)[0]
ViennaRNA.svg_rna_plot(seq, structure, 'structure.svg')