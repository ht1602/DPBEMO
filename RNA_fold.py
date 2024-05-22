import RNA
import time

def RNA_inverse(sequence, target):
    current_structure, obj1, _ = predict_structure(sequence)
    print(current_structure)
    obj2 = str_similarity(current_structure, target)
    print(obj1, 1-obj2)
    return [obj1, 1 - obj2]


def predict_structure(sequence):
    """
    Predict the secondary structure and minimum free energy of an RNA sequence using ViennaRNA's Python API.
    Parameters:
    - sequence: The RNA sequence, in string format.
    Returns:
    - The structure and free energy.

    # test
    sequence = "GCGCUUCGCCGCGCCG"
    predict_structure(sequence)
    """
    # Create an RNA folding object
    fold_compound = RNA.fold_compound(sequence)
    # Calculate the secondary structure and minimum free energy
    start_time = time.time()
    (structure, mfe) = fold_compound.mfe()
    # End timing
    end_time = time.time()
    # Compute total time
    computation_time = end_time - start_time
    # Print the results
    # print(f"Sequence: {sequence}")
    # print(f"Structure: {structure}")
    # print(f"Minimum Free Energy: {mfe} kcal/mol")
    return structure, mfe, computation_time

def str_distance(s1, s2):
    """Calculate the Hamming distance between two strings"""
    dis= sum(el1 != el2 for el1, el2 in zip(s1, s2))
    return dis

def str_similarity(s1, s2):
    """Calculate the similarity between two strings"""
    dis= sum(el1 != el2 for el1, el2 in zip(s1, s2))
    N = sum(1 for el1 in s1)
    print("structure length is ", N)
    similarity = (float) (N - dis) / N
    return similarity


def get_str(row):
    # get structure from data
    sequence_length = row['length']
    pairs = row['pairs']
    # Initialising a braces representation string
    dot_bracket = ['.' for _ in range(sequence_length)]
    # 填充配对信息
    for i, j, _ in pairs:
        dot_bracket[i] = '('
        dot_bracket[j] = ')'
    # Converting lists to strings
    dot_bracket_str = ''.join(dot_bracket)
    return dot_bracket_str

if __name__=="__main__":
    target_structure = "((((((.((((((((....))))).)).).))))))"
    sequence = "GCACCCAGUGUGGCGGACACGCCACACUGGGUGCGU"
    obj1, obj2 = RNA_inverse(sequence, target_structure)
    print(obj1, obj2)