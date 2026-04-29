import streamlit as st
import pymysql


# ------------------ SESSION STATE ------------------
if "logged_in" not in st.session_state:
    st.session_state.logged_in = False
if "customer_id" not in st.session_state:
    st.session_state.customer_id = None
if "page" not in st.session_state:
    st.session_state.page = "home"
if "customer_name" not in st.session_state:
    st.session_state.customer_name = ""
if "role" not in st.session_state:
    st.session_state.role = None
if "customer_city" not in st.session_state:
    st.session_state.customer_city = ""


# ------------------ DB CONNECTION ------------------
conn = pymysql.connect(
    host="localhost",
    user="root",
    password="Mysql@2026",
    database="agri_supply_chain"
)
cursor = conn.cursor()


# ------------------ UI STYLING ------------------
st.markdown("""
<style>
.stApp {
    background: linear-gradient(rgba(0,80,0,0.5), rgba(0,120,100,0.4)),
                url("https://images.unsplash.com/photo-1500382017468-9049fed747ef");
    background-size: cover;
}
div[data-testid="stForm"] {
    background: linear-gradient(rgba(253,246,227,0.95), rgba(245,236,210,0.95));
    padding: 40px;
    border-radius: 20px;
    max-width: 420px;
    margin: auto;
    margin-top: 40px;
}
h1 {
    text-align: center;
    color: #f1f8e9;
}
.stTextInput>div>div>input {
    background-color: #f8f3e6 !important;
    border-radius: 12px;
}
.stButton>button {
    background: linear-gradient(135deg, #2e7d32, #66bb6a);
    color: white;
    border-radius: 12px;
    height: 50px;
    font-size: 16px;
}
.success-box,.error-box,.warning-box {
    padding: 12px;
    border-radius: 10px;
    text-align: center;
    font-weight: 600;
    margin: 10px auto;
    max-width: 420px;
}
.success-box { background:#1b5e20; color:white; }
.error-box { background:#b71c1c; color:white; }
.warning-box { background:#f57c00; color:white; }
</style>
""", unsafe_allow_html=True)


# ------------------ TITLE ------------------
st.markdown("<h1>🌿 Agri Supply Chain System</h1>", unsafe_allow_html=True)
message = st.empty()


# ------------------ LOGIN ------------------
if not st.session_state.logged_in:


    login_type = st.radio("", ["Customer", "Admin"], horizontal=True)
    title = "Customer Login / Signup" if login_type == "Customer" else "Admin Login"
    st.markdown(f"<h3 style='text-align:center;'>{title}</h3>", unsafe_allow_html=True)


    with st.form("login_form"):


        if login_type == "Customer":
            name = st.text_input("Customer Name")
            contact = st.text_input("Contact Number")
            city = st.text_input("City (only for new users)")
        else:
            name = st.text_input("Admin Username")
            contact = st.text_input("Password", type="password")
            city = ""


        submit = st.form_submit_button("Login")


    if submit:
        if not name or not contact:
            message.markdown("<div class='error-box'>Please fill details</div>", unsafe_allow_html=True)


        elif login_type == "Admin":
            if name.lower() == "admin" and contact == "admin123":
                st.session_state.logged_in = True
                st.session_state.role = "admin"
                st.rerun()
            else:
                message.markdown("<div class='error-box'>Invalid admin credentials</div>", unsafe_allow_html=True)


        else:
            cursor.execute("""
                SELECT customer_id, city
                FROM customer
                WHERE lower(customer_name)=%s AND contact_no=%s
            """, (name.lower(), contact))
            result = cursor.fetchone()


            if result:
                st.session_state.logged_in = True
                st.session_state.role = "customer"
                st.session_state.customer_id = result[0]
                st.session_state.customer_city = result[1]
                st.session_state.customer_name = name
                st.session_state.page = "home"
                st.rerun()


            else:
                if city.strip():
                    cursor.execute("""
                        INSERT INTO customer(customer_name, contact_no, city)
                        VALUES(%s,%s,%s)
                    """, (name.lower(), contact, city))
                    conn.commit()


                    cursor.execute("""
                        SELECT customer_id, city
                        FROM customer
                        WHERE contact_no=%s
                    """, (contact,))
                    row = cursor.fetchone()


                    st.session_state.customer_id = row[0]
                    st.session_state.customer_city = row[1]
                    st.session_state.customer_name = name
                    st.session_state.logged_in = True
                    st.session_state.role = "customer"
                    st.session_state.page = "home"
                    st.rerun()
                else:
                    message.markdown("<div class='warning-box'>Enter city for signup</div>", unsafe_allow_html=True)


# ------------------ CUSTOMER DASHBOARD ------------------
if st.session_state.logged_in and st.session_state.role == "customer":


    if st.session_state.page == "home":


        st.markdown(
            f"<h3 style='text-align:center;'>Welcome 👋<br>{st.session_state.customer_name.capitalize()}</h3>",
            unsafe_allow_html=True
        )


        col1, col2, col3 = st.columns(3)


        with col1:
            if st.button("📝 Register Complaint"):
                st.session_state.page = "register"


        with col2:
            if st.button("🔍 Check Status"):
                st.session_state.page = "status"


        with col3:
            if st.button("⭐ Give Review"):
                st.session_state.page = "review"


        st.markdown("---")


        if st.button("🚪 Logout"):
            st.session_state.clear()
            st.rerun()


    # ------------------ REGISTER COMPLAINT ------------------
    elif st.session_state.page == "register":


        st.markdown("### 📝 Register Complaint")


        try:
            cursor.execute("""
                SELECT retailer_name
                FROM retailer
                WHERE lower(trim(village)) = lower(trim(%s))
                ORDER BY retailer_name
            """, (st.session_state.customer_city,))
            retailer_list = [row[0] for row in cursor.fetchall()]


            if len(retailer_list) == 0:
                st.markdown(
                    "<div class='warning-box'>No retailer found for your city</div>",
                    unsafe_allow_html=True
                )


            else:
                retailer = st.selectbox("Retailer Name", retailer_list)


                cursor.execute("""
                    SELECT crop_name
                    FROM crop
                    ORDER BY crop_name
                """)
                crop_list = [row[0] for row in cursor.fetchall()]
                crop = st.selectbox("Crop Name", crop_list)


                date = st.text_input("Date (YYYY-MM-DD)")
                text = st.text_area("Complaint Description")


                if st.button("Submit Complaint"):


                    if retailer and crop and date and text:


                        # Check purchase relation only for validation purpose
                        cursor.execute("""
                            SELECT rc.customer_id
                            FROM retailer_customer rc
                            JOIN retailer r ON rc.retailer_id = r.retailer_id
                            JOIN harvest_batch hb ON rc.batch_id = hb.batch_id
                            JOIN cultivation cu ON hb.cultivation_id = cu.cultivation_id
                            JOIN crop cr ON cu.crop_id = cr.crop_id
                            WHERE rc.customer_id=%s
                            AND lower(r.retailer_name)=%s
                            AND lower(cr.crop_name)=%s
                            AND rc.date_of_purchase=%s
                        """, (
                            st.session_state.customer_id,
                            retailer.lower(),
                            crop.lower(),
                            date
                        ))


                        result = cursor.fetchone()


                        # Always insert complaint
                        cursor.execute("""
                            INSERT INTO complaint(
                                customer_id,
                                retailer_name,
                                crop_name,
                                date_of_purchase,
                                complaint_text,
                                status
                            )
                            VALUES(%s,%s,%s,%s,%s,%s)
                        """, (
                            st.session_state.customer_id,
                            retailer.lower(),
                            crop.lower(),
                            date,
                            text,
                            "pending"
                        ))
                        conn.commit()


                        # 🔥 REMOVED MANUAL UPDATE QUERY
                        # Trigger automatically updates harvest_batch.status


                        st.markdown(
                            "<div class='success-box'>Complaint registered successfully</div>",
                            unsafe_allow_html=True
                        )


                    else:
                        st.markdown(
                            "<div class='error-box'>Fill all fields</div>",
                            unsafe_allow_html=True
                        )


        except Exception as e:
            st.markdown(
                f"<div class='error-box'>Error: {str(e)}</div>",
                unsafe_allow_html=True
            )


        if st.button("⬅ Back"):
            st.session_state.page = "home"


    # ------------------ REVIEW ------------------
    elif st.session_state.page == "review":


        st.markdown("### ⭐ Submit Review")


        cursor.execute("""
            SELECT retailer_name
            FROM retailer
            WHERE lower(trim(village)) = lower(trim(%s))
            ORDER BY retailer_name
        """, (st.session_state.customer_city,))
        retailer_list = [row[0] for row in cursor.fetchall()]


        if len(retailer_list) == 0:
            st.warning("No retailers found in your city")


        else:
            retailer = st.selectbox("Retailer Name", retailer_list)


            cursor.execute("""
                SELECT crop_name
                FROM crop
                ORDER BY crop_name
            """)
            crop_list = [row[0] for row in cursor.fetchall()]
            crop = st.selectbox("Crop Name", crop_list)


            date = st.text_input("Date of Purchase (YYYY-MM-DD)")
            rating = st.slider("Rating", 1, 5)
            review_text = st.text_area("Review Text")


            if st.button("Submit Review"):


                cursor.execute("""
                    SELECT rc.batch_id
                    FROM retailer_customer rc
                    JOIN retailer r ON rc.retailer_id = r.retailer_id
                    JOIN harvest_batch hb ON rc.batch_id = hb.batch_id
                    JOIN cultivation cu ON hb.cultivation_id = cu.cultivation_id
                    JOIN crop cr ON cu.crop_id = cr.crop_id
                    WHERE rc.customer_id=%s
                    AND lower(r.retailer_name)=%s
                    AND lower(cr.crop_name)=%s
                    AND rc.date_of_purchase=%s
                """, (
                    st.session_state.customer_id,
                    retailer.lower(),
                    crop.lower(),
                    date
                ))


                batch = cursor.fetchone()


                if not batch:
                    st.markdown("<div class='error-box'>No matching purchase record found</div>", unsafe_allow_html=True)


                else:
                    batch_id = batch[0]


                    cursor.execute("""
                        INSERT INTO review(customer_id, batch_id, rating, review_text)
                        VALUES (%s,%s,%s,%s)
                    """, (
                        st.session_state.customer_id,
                        batch_id,
                        rating,
                        review_text
                    ))
                    conn.commit()


                    cursor.execute("CALL update_batch_quality(%s)", (batch_id,))
                    conn.commit()


                    st.markdown(
                        "<div class='success-box'>Review submitted and batch quality updated</div>",
                        unsafe_allow_html=True
                    )


        if st.button("⬅ Back"):
            st.session_state.page = "home"


    # ------------------ STATUS ------------------
    elif st.session_state.page == "status":


        st.markdown("### 🔍 Complaint Status")


        cursor.execute("""
            SELECT complaint_id, retailer_name, crop_name, status
            FROM complaint
            WHERE customer_id=%s
        """, (st.session_state.customer_id,))


        for row in cursor.fetchall():
            st.write(row)


        if st.button("⬅ Back"):
            st.session_state.page = "home"


# ------------------ ADMIN DASHBOARD ------------------
if st.session_state.logged_in and st.session_state.role == "admin":


    st.markdown("<h3 style='text-align:center;'>Admin Panel</h3>", unsafe_allow_html=True)


    cursor.execute("SELECT * FROM complaint")
    for row in cursor.fetchall():
        st.write(row)


    cid = st.text_input("Enter Complaint ID")
    new_status = st.selectbox(
        "Update Status",
        ["pending", "under review", "resolved", "rejected"]
    )


    if st.button("Update"):
        cursor.execute("""
            UPDATE complaint
            SET status=%s
            WHERE complaint_id=%s
        """, (new_status, cid))
        conn.commit()
        st.success("Updated")


    if st.button("Logout"):
        st.session_state.clear()
        st.rerun()


# ------------------ CLOSE ------------------
cursor.close()
conn.close()
