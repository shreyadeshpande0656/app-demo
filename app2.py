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
    password="mysqlroot",
    database="agriculture"
)
cursor = conn.cursor()




# ------------------ UI STYLING ------------------
st.markdown("""
<style>


/* Background */
.stApp {
    background: linear-gradient(rgba(0,60,40,0.6), rgba(0,90,70,0.6)),
                url("https://images.unsplash.com/photo-1500382017468-9049fed747ef");
    background-size: cover;
    font-family: 'Segoe UI', sans-serif;
}


/* Glass Card */
div[data-testid="stForm"] {
    backdrop-filter: blur(16px);
    <div class='feature-card'>
    border: 1px solid rgba(255,255,255,0.25);
    padding: 40px;
    border-radius: 20px;
    max-width: 420px;
    margin: auto;
    margin-top: 30px;
    box-shadow: 0 10px 40px rgba(0,0,0,0.3);
    animation: floatUp 0.6s ease;
}


@keyframes floatUp {
    from {opacity: 0; transform: translateY(30px);}
    to {opacity: 1; transform: translateY(0);}
}


/* Title */
h1 {
    text-align: center;
    color: #e8f5e9;
    font-size: 42px;
    letter-spacing: 1px;
}


/* Subtitle */
h3 {
    color: #f1f8e9;
    margin-bottom: 20px;
}


/* Inputs */
.stTextInput>div>div>input {
    background-color: rgba(255,255,255,0.9) !important;
    color: #1b1b1b !important;   /* 🔥 TEXT COLOR FIX */
    border-radius: 10px;
    padding: 10px;
    border: none;
    font-weight: 500;
}


/* Placeholder text */
.stTextInput>div>div>input::placeholder {
    color: #666 !important;
}


/* Buttons */
.stButton>button {
    background: linear-gradient(135deg, #2e7d32, #66bb6a);
    color: white;
    border-radius: 12px;
    height: 50px;
    font-size: 16px;
    transition: all 0.3s ease;
    border: none;
}


/* Button Hover */
.stButton>button:hover {
    transform: translateY(-3px);
    box-shadow: 0 8px 20px rgba(0,0,0,0.3);
}


/* Message Boxes */
.success-box,.error-box,.warning-box {
    padding: 14px;
    border-radius: 12px;
    text-align: center;
    font-weight: 600;
    margin: 15px auto;
    max-width: 420px;
    animation: fadeIn 0.5s ease-in-out;
}


.success-box { background:#2e7d32; color:white; }
.error-box { background:#c62828; color:white; }
.warning-box { background:#f9a825; color:black; }


/* Animations */
@keyframes fadeIn {
    from {opacity: 0; transform: translateY(10px);}
    to {opacity: 1; transform: translateY(0);}
}


/* Allow clicks to pass through background layers */
.stApp {
    pointer-events: auto;
}


/* Ensure form is fully clickable */
div[data-testid="stForm"] {
    position: relative;
    z-index: 10;
}


/* Fix for all interactive elements */
button, input, textarea {
    position: relative;
    z-index: 20;
}


/* Input focus glow */
.stTextInput>div>div>input:focus {
    outline: none;
    box-shadow: 0 0 0 2px rgba(102,187,106,0.6);
}


/* -------- FEATURE CARD -------- */


.feature-card {
    background: rgba(255,255,255,0.1);
    padding: 35px 25px;
    border-radius: 18px;
    text-align: center;
    transition: all 0.3s ease;
    cursor: pointer;


    min-height: 230px;   /* 🔥 makes cards taller */
}


/* Prevent ugly word breaking */
.feature-card h3 {
    white-space: normal;
    word-break: keep-all;
}


/* Hover effect */
.feature-card:hover {
    transform: translateY(-10px) scale(1.03);
    box-shadow: 0 15px 40px rgba(0,0,0,0.4);
    background: rgba(255,255,255,0.18);
}


/* Headings hover */
h3:hover {
    color: #a5d6a7;
    transition: 0.3s;
}


/* -------- TITLE ALIGNMENT -------- */


.card-title {
    display: flex;
    align-items: center;
    justify-content: center;
    gap: 12px;


    font-size: 22px;
    font-weight: 600;
    color: #ffffff;


    white-space: nowrap;   /* 🔥 PREVENT LINE BREAK */
}




/* Icon size */
.card-title .icon {
    font-size: 24px;
}


/* Fix text alignment */
.feature-card p {
    text-align: center;
    line-height: 1.5;
}


</style>
""", unsafe_allow_html=True)




# ------------------ TITLE ------------------
st.markdown(
    "<h1 style='text-align:center; color:#e8f5e9;'>🌿 AgriLink</h1>",
    unsafe_allow_html=True
)


st.markdown(
    "<p style='text-align:center; color:#c8e6c9; font-size:18px;'>"
    "Connecting farmers, suppliers, and customers through a smarter supply chain."
    "</p>",
    unsafe_allow_html=True
)


st.write("")
st.write("")


if st.session_state.logged_in:
    col1, col2, col3 = st.columns(3)


    with col1:
        st.markdown("""
        <div class='feature-card'>
        <div class='card-title'>
            <span class='icon'>📦</span>
            <span>Track Orders</span>
        </div>
        <p>Real-time supply tracking from farm to delivery.</p>
        </div>
        """, unsafe_allow_html=True)


        st.markdown("**Register Complaint 📝**")
        if st.button("Register", key="btn1"):
            st.session_state.page = "register"




    with col2:
        st.markdown("""
        <div class='feature-card'>
        <div class='card-title'>
            <span class='icon'>📝</span>
            <span>Easy Complaints</span>
        </div>
        <p>Register and resolve issues quickly.</p>
        </div>
        """, unsafe_allow_html=True)


        st.markdown("**Check Status 🔍**")
        if st.button("Status", key="btn2"):
            st.session_state.page = "status"




    with col3:
        st.markdown("""
        <div class='feature-card'>
        <div class='card-title'>
            <span class='icon'>⭐</span>
            <span>Feedback System</span>
        </div>
        <p>Improve services with customer insights.</p>
        </div>
        """, unsafe_allow_html=True)


        st.markdown("**Give Review ⭐**")
        if st.button("Review", key="btn3"):
            st.session_state.page = "review"


# ------------------ LOGIN ------------------
if not st.session_state.logged_in:






    st.markdown(
    "<p style='font-size:20px; font-weight:600; color:#e8f5e9;'>Select Login Type</p>",
    unsafe_allow_html=True
)


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


    unsafe_allow_html=True


    st.markdown("<hr style='border: 1px solid rgba(255,255,255,0.2);'>", unsafe_allow_html=True)


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




