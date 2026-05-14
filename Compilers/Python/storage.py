import pandas as pd
import os
from config import CSV_FILE, CSV_COLUMNS, log


def load_csv():
    if not os.path.exists(CSV_FILE):
        return pd.DataFrame(columns=CSV_COLUMNS)
    try:
        df = pd.read_csv(CSV_FILE)
        # Διασφαλίζουμε ότι έχουμε όλες τις απαραίτητες στήλες, ακόμα και αν το αρχείο είναι παλιό
        for col in CSV_COLUMNS:
            if col not in df.columns:
                df[col] = ""
        return df[CSV_COLUMNS]
    except Exception as e:
        log("Storage", "Error", str(e))
        return pd.DataFrame(columns=CSV_COLUMNS)


def save_courses(new_courses):
    if not new_courses:
        return
    # Φορτώνω τα παλιά και προσθέτω τα νέα σε append mode, χωρίς να χάνω παλιά δεδομένα
    df_old = load_csv()
    df_new = pd.DataFrame(new_courses)
    for col in CSV_COLUMNS:
        if col not in df_new.columns:
            df_new[col] = ""
    df_new = df_new[CSV_COLUMNS]
    df_combined = pd.concat([df_old, df_new], ignore_index=True).infer_objects(copy=False)
    # Αφαιρούμε διπλότυπα με βάση τίτλο + πάροχο, κρατάμε το πιο πρόσφατο
    df_combined = df_combined.drop_duplicates(subset=["title", "provider"], keep="last")
    df_combined.to_csv(CSV_FILE, index=False)
    log("Storage", "Success", f"{len(df_new)} νέα, {len(df_combined)} σύνολο στο {CSV_FILE}")


def export_csv(filepath):
    df = load_csv()
    df.to_csv(filepath, index=False)
    log("Export", "Success", filepath)
