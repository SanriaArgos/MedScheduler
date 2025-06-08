export const metadata = {
    title: "Документация API эндпоинтов | Med Scheduler", description: "На этой странице представлен список API эндпоинтов сервера с кратким описанием",
};

export default function ApiDocsPage() {
    return <main className="prose prose-lg max-w-5xl mx-auto px-4 py-10">
        <h2>Документация API эндпоинтов</h2>
        <p>Ниже представлен список API эндпоинтов сервера с кратким описанием.</p>
        <hr/>

        <h3>Аутентификация и Регистрация</h3>
        <ol>
            <li>
                <b>POST <code>/login</code></b>
                <ul>
                    <li><b>Описание:</b> Аутентификация пользователя.</li>
                    <li><b>Request Body (JSON):</b> <code>{'{"phone": "string", "password": "string"}'}</code></li>
                    <li><b>Response Body (Success - 200 OK, JSON):</b>
                        <code>{'{"success": true, "action": "login", "result": "USER_TYPE:USER_ID"}'}</code> (например, <code>"patient:123"</code>, <code>"doctor:45"</code>)
                    </li>
                    <li><b>Response Body (Error - 401 Unauthorized, JSON):</b>
                        <code>{'{"success": false, "action": "login", "error": "Invalid credentials"}'}</code></li>
                    <li><b>Response Body (Error - 400 Bad Request, JSON):</b>
                        <code>{'{"error": "Missing phone or password"}'}</code></li>
                </ul>
            </li>
            <li>
                <b>POST <code>/registration</code></b>
                <ul>
                    <li><b>Описание:</b> Регистрация нового пользователя (по умолчанию как пациента).</li>
                    <li><b>Request Body (JSON):</b>
                        <code>{'{"last_name": "string", "first_name": "string", "patronymic": "string" (optional), "phone": "string", "password": "string"}'}</code>
                    </li>
                    <li><b>Response Body (Success - 200 OK, JSON):</b>
                        <code>{'{"success": true, "action": "registration"}'}</code></li>
                    <li><b>Response Body (Error - 409 Conflict, JSON):</b>
                        <code>{'{"success": false, "action": "registration", "error": "User already exists or registration failed"}'}</code>
                    </li>
                    <li><b>Response Body (Error - 400 Bad Request, JSON):</b>
                        <code>{'{"success": false, "error": "Missing required fields"}'}</code></li>
                </ul>
            </li>
        </ol>

        <hr/>

        <h3>Управление Пользователями (Административные)</h3>
        <ol start={3}>
            <li>
                <b>POST <code>/add_junior_admin</code></b> <i>(Для Senior Admin)</i>
                <ul>
                    <li><b>Описание:</b> Добавление нового младшего администратора.</li>
                    <li>
                        <b>Request Body (JSON):</b>
                        <code>{'{"last_name": "string", "first_name": "string", "patronymic": "string" (optional), "phone": "string"}'}</code> (пароль
                        по умолчанию "0987654321")
                    </li>
                    <li><b>Response Body (Success - 200 OK, JSON):</b>
                        <code>{'{"success": true, "message": "Junior administrator successfully added"}'}</code></li>
                    <li><b>Response Body (Error, JSON):</b>
                        <code>{'{"success": false, "error": "сообщение об ошибке"}'}</code> (статусы 400, 409, 500)
                    </li>
                </ul>
            </li>
            <li>
                <b>DELETE <code>/delete_user_by_phone?phone=&lt;phone_number&gt;</code></b> <i>(Для Senior Admin)</i>
                <ul>
                    <li><b>Описание:</b> Удаление пользователя по номеру телефона.</li>
                    <li><b>Request Parameters:</b> <code>phone</code> (string)</li>
                    <li><b>Response Body (Success - 200 OK, JSON):</b> <code>{'{"success": true}'}</code></li>
                    <li><b>Response Body (Error - 404 Not Found, JSON):</b>
                        <code>{'{"success": false, "error": "User not found"}'}</code></li>
                    <li><b>Response Body (Error, JSON):</b>
                        <code>{'{"success": false, "error": "сообщение об ошибке"}'}</code> (статусы 400, 500)
                    </li>
                    <li><b>Примечание:</b> Клиент должен использовать HTTP DELETE.</li>
                </ul>
            </li>
            <li>
                <b>GET <code>/get_users</code></b> <i>(Для Junior/Senior Admin)</i>
                <ul>
                    <li><b>Описание:</b> Получение списка всех пользователей.</li>
                    <li><b>Response Body (Success - 200 OK, JSON):</b>
                        <code>{'{"success": true, "users": [{"id": "int", "last_name": "string", ...}]}'}</code></li>
                    <li><b>Response Body (Error - 500 Internal Server Error, JSON):</b>
                        <code>{'{"success": false, "error": "сообщение об ошибке"}'}</code></li>
                </ul>
            </li>
        </ol>

        <hr/>

        <h3>Управление Профилями Пользователей</h3>
        <ol start={6}>
            <li>
                <b>PATCH <code>/edit_patient_profile</code></b>
                <ul>
                    <li><b>Описание:</b> Редактирование профиля пациента.</li>
                    <li>
                        <b>Request Body (JSON):</b>
                        <code>{'{ "user_id": int, "current_password": "string", "last_name": "string" (opt), "first_name": "string" (opt), "patronymic": "string" (opt), "phone": "string" (opt), "new_password": "string" (opt), "new_password_repeat": "string" (opt, required if new_password) }'}</code>
                    </li>
                    <li><b>Response Body (Success - 200 OK, JSON):</b>
                        <code>{'{"success": true, "message": "Profile updated successfully"}'}</code></li>
                    <li><b>Response Body (Error, JSON):</b>
                        <code>{'{"success": false, "error": "сообщение об ошибке"}'}</code> (статусы 400, 401, 403, 404,
                        409, 500)
                    </li>
                </ul>
            </li>
            <li>
                <b>PATCH <code>/edit_doctor_profile</code></b>
                <ul>
                    <li><b>Описание:</b> Редактирование профиля врача.</li>
                    <li><b>Request Body (JSON):</b>
                        <code>{'{ "user_id": int, "current_password": "string", "last_name": "string" (opt), "first_name": "string" (opt), "patronymic": "string" (opt), "phone": "string" (opt), "new_password": "string" (opt), "new_password_repeat": "string" (opt, required if new_password) }'}</code>
                    </li>
                    <li><b>Response Body (Success - 200 OK, JSON):</b>
                        <code>{'{"success": true, "message": "Profile updated successfully"}'}</code></li>
                    <li><b>Response Body (Error, JSON):</b>
                        <code>{'{"success": false, "error": "сообщение об ошибке"}'}</code> (статусы 400, 401, 403, 404,
                        409, 500)
                    </li>
                </ul>
            </li>
            <li>
                <b>PATCH <code>/edit_junior_admin_profile</code></b>
                <ul>
                    <li><b>Описание:</b> Редактирование профиля младшего администратора. Телефон менять нельзя.</li>
                    <li>
                        <b>Request Body (JSON):</b>
                        <code>{'{ "user_id": int, "current_password": "string", "last_name": "string" (opt), "first_name": "string" (opt), "patronymic": "string" (opt), "new_password": "string" (opt), "new_password_repeat": "string" (opt, required if new_password) }'}</code>
                    </li>
                    <li><b>Response Body (Success - 200 OK, JSON):</b>
                        <code>{'{"success": true, "message": "Junior admin profile updated successfully"}'}</code></li>
                    <li><b>Response Body (Error, JSON):</b>
                        <code>{'{"success": false, "error": "сообщение об ошибке"}'}</code> (статусы 400, 401, 403, 404,
                        409, 500)
                    </li>
                </ul>
            </li>
            <li>
                <b>PATCH <code>/edit_senior_admin_profile</code></b>
                <ul>
                    <li><b>Описание:</b> Редактирование профиля старшего администратора.</li>
                    <li><b>Request Body (JSON):</b>
                        <code>{'{ "user_id": int, "current_password": "string", "last_name": "string" (opt), "first_name": "string" (opt), "patronymic": "string" (opt), "phone": "string" (opt), "new_password": "string" (opt), "new_password_repeat": "string" (opt, required if new_password) }'}</code>
                    </li>
                    <li><b>Response Body (Success - 200 OK, JSON):</b>
                        <code>{'{"success": true, "message": "Senior admin profile updated successfully"}'}</code></li>
                    <li><b>Response Body (Error, JSON):</b>
                        <code>{'{"success": false, "error": "сообщение об ошибке"}'}</code> (статусы 400, 401, 403, 404,
                        409, 500)
                    </li>
                </ul>
            </li>
            <li>
                <b>DELETE <code>/delete_self_account?user_id=&lt;user_id&gt;</code></b> <i>(Для Patient, Doctor)</i>
                <ul>
                    <li><b>Описание:</b> Удаление собственного аккаунта.</li>
                    <li><b>Request Parameters:</b> <code>user_id</code> (int)</li>
                    <li><b>Response Body (Success - 200 OK, JSON):</b> <code>{'{"success": true}'}</code></li>
                    <li><b>Response Body (Error, JSON):</b>
                        <code>{'{"success": false, "error": "сообщение об ошибке"}'}</code> (статусы 403, 404, 500)
                    </li>
                </ul>
            </li>
        </ol>

        <hr/>

        <h3>Управление Врачами (Административные)</h3>
        <ol start={11}>
            <li>
                <b>POST <code>/add_doctor</code></b> <i>(Для Junior Admin)</i>
                <ul>
                    <li><b>Описание:</b> Добавление нового врача.</li>
                    <li>
                        <b>Request Body (JSON):</b>
                        <code>{'{"last_name": "string", "first_name": "string", "patronymic": "string" (opt), "phone": "string", "education": "string", "specialty": "string", "experience": int, "price": int}'}</code> (пароль
                        по умолчанию "987654321")
                    </li>
                    <li><b>Response Body (Success - 200 OK, JSON):</b>
                        <code>{'{"success": true, "message": "Doctor successfully added"}'}</code></li>
                    <li><b>Response Body (Error, JSON):</b>
                        <code>{'{"success": false, "error": "сообщение об ошибке"}'}</code> (статусы 400, 409, 500)
                    </li>
                </ul>
            </li>
            <li>
                <b>GET <code>/get_doctors</code></b> <i>(Для Junior/Senior Admin)</i>
                <ul>
                    <li><b>Описание:</b> Получение списка всех врачей.</li>
                    <li><b>Response Body (Success - 200 OK, JSON):</b>
                        <code>{'{"success": true, "doctors": [{"doctor_id": "int", ...}]}'}</code></li>
                    <li><b>Response Body (Error - 500 Internal Server Error, JSON):</b>
                        <code>{'{"success": false, "error": "сообщение об ошибке"}'}</code></li>
                </ul>
            </li>
            <li>
                <b>GET <code>/doctors_exist/&lt;doctor_id&gt;</code></b> <i>(Для Junior Admin)</i>
                <ul>
                    <li><b>Описание:</b> Проверка существования врача по ID.</li>
                    <li><b>Response Body (Success - 200 OK, JSON):</b>
                        <code>{'{"success": true, "exists": true, "doctor_id": "string"}'}</code></li>
                    <li><b>Response Body (Success - 404 Not Found, JSON):</b>
                        <code>{'{"success": true, "exists": false}'}</code></li>
                    <li><b>Response Body (Error, JSON):</b>
                        <code>{'{"success": false, "error": "сообщение об ошибке"}'}</code></li>
                </ul>
            </li>
        </ol>

        <hr/>

        <h3>Управление Больницами (Административные)</h3>
        <ol start={14}>
            <li>
                <b>POST <code>/add_hospital</code></b> <i>(Для Senior Admin)</i>
                <ul>
                    <li><b>Описание:</b> Добавление новой больницы. <code>admin_id</code> -
                        это <code>user_id</code> младшего администратора.
                    </li>
                    <li>
                        <b>Request Body (JSON):</b>
                        <code>{'{"region": "string", "settlement_type": "string", "settlement_name": "string", "street": "string", "house": "string", "full_name": "string", "admin_id": "string" (user_id младшего админа)}'}</code>
                    </li>
                    <li><b>Response Body (Success - 200 OK, JSON):</b>
                        <code>{'{"success": true, "message": "Hospital successfully added"}'}</code></li>
                    <li><b>Response Body (Error, JSON):</b>
                        <code>{'{"success": false, "error": "сообщение об ошибке"}'}</code> (статусы 400, 403, 404, 409,
                        500)
                    </li>
                </ul>
            </li>
            <li>
                <b>GET <code>/get_hospitals</code></b> <i>(Для Senior Admin)</i>
                <ul>
                    <li><b>Описание:</b> Получение списка всех больниц.</li>
                    <li><b>Response Body (Success - 200 OK, JSON):</b>
                        <code>{'{"success": true, "hospitals": [{"hospital_id": "string", ...}]}'}</code></li>
                    <li><b>Response Body (Error - 500 Internal Server Error, JSON):</b>
                        <code>{'{"success": false, "error": "сообщение об ошибке"}'}</code></li>
                </ul>
            </li>
            <li>
                <b>GET <code>/hospitals_exist/&lt;hospital_id&gt;</code></b> <i>(Для Junior Admin)</i>
                <ul>
                    <li><b>Описание:</b> Проверка существования больницы по ID.</li>
                    <li><b>Response Body (Success - 200 OK, JSON):</b>
                        <code>{'{"success": true, "exists": true, "hospital_id": "string"}'}</code></li>
                    <li><b>Response Body (Success - 404 Not Found, JSON):</b>
                        <code>{'{"success": true, "exists": false}'}</code></li>
                    <li><b>Response Body (Error, JSON):</b>
                        <code>{'{"success": false, "error": "сообщение об ошибке"}'}</code></li>
                </ul>
            </li>
            <li>
                <b>POST <code>/attach_doctor_to_hospital</code></b> <i>(Для Junior Admin)</i>
                <ul>
                    <li><b>Описание:</b> Прикрепление врача к больнице.</li>
                    <li>
                        <b>Request Body (JSON):</b>
                        <code>{'{"doctor_id": int, "hospital_id": int, "junior_admin_id": int}'}</code>
                    </li>
                    <li><b>Response Body (Success - 200 OK, JSON):</b>
                        <code>{'{"success": true, "message": "Doctor successfully attached to hospital"}'}</code></li>
                    <li><b>Response Body (Error, JSON):</b>
                        <code>{'{"success": false, "error": "сообщение об ошибке"}'}</code> (статусы 400, 409, 500, 503)
                    </li>
                </ul>
            </li>
            <li>
                <b>POST <code>/detach_doctor_from_hospital</code></b> <i>(Для Junior Admin)</i>
                <ul>
                    <li><b>Описание:</b> Открепление врача от больницы.</li>
                    <li>
                        <b>Request Body (JSON):</b>
                        <code>{'{"doctor_id": int, "hospital_id": int, "junior_admin_id": int}'}</code>
                    </li>
                    <li><b>Response Body (Success - 200 OK, JSON):</b>
                        <code>{'{"success": true, "message": "Hospital ID removed from doctor\'s list"}'}</code></li>
                            <li><b>Response Body (Error, JSON):</b> <code>{'{"success": false, "error": "сообщение об ошибке"}'}</code> (статусы
                        400, 403, 404, 500)
                    </li>
                </ul>
            </li>
        </ol>

        <hr/>

        <h3>Управление Записями на Прием и Расписанием</h3>
        <ol start={19}>
            <li>
                <b>POST <code>/add_record_slot</code></b> <i>(Для Junior Admin)</i>
                <ul>
                    <li><b>Описание:</b> Добавление нового слота для записи к
                        врачу. <code>junior_admin_id</code> используется для проверки, что больница принадлежит этому
                        админу.
                    </li>
                    <li>
                        <b>Request Body (JSON):</b>
                        <code>{'{"doctor_id": int, "date": "YYYY-MM-DD", "time": "HH:MM:SS", "hospital_id": int, "cabinet": int, "junior_admin_id": int}'}</code>
                    </li>
                    <li><b>Response Body (Success - 200 OK, JSON):</b>
                        <code>{'{"success": true, "message": "Appointment slot added"}'}</code></li>
                    <li><b>Response Body (Error, JSON):</b>
                        <code>{'{"success": false, "error": "сообщение об ошибке"}'}</code> (статусы 400, 403, 404, 500)
                    </li>
                </ul>
            </li>
            <li>
                <b>GET <code>/get_doctor_schedule?doctor_id=&lt;id&gt;</code></b> <i>(Для Doctor, Junior Admin)</i>
                <ul>
                    <li><b>Описание:</b> Получение расписания врача на ближайшие 7 дней. Для Junior Admin также
                        выполняется проверка принадлежности врача к его больнице
                        (через <code>/check_doctor_admin_hospital</code>).
                    </li>
                    <li><b>Request Parameters:</b> <code>doctor_id</code> (int)</li>
                    <li><b>Response Body (Success - 200 OK, JSON):</b>
                        <code>{'{"success": true, "schedule": [{"appointment_date": "YYYY-MM-DD", "appointment_time": "HH:MM:SS", "full_name": "string" (название больницы), ...}]}'}</code>
                    </li>
                    <li><b>Response Body (Error, JSON):</b>
                        <code>{'{"success": false, "error": "сообщение об ошибке"}'}</code></li>
                </ul>
            </li>
            <li>
                <b>GET <code>/get_doctor_schedule_for_patient?doctor_id=&lt;id&gt;</code></b> <i>(Для Patient)</i>
                <ul>
                    <li><b>Описание:</b> Получение расписания врача для пациента (включая статус слота
                        и <code>hospital_id</code>).
                    </li>
                    <li><b>Request Parameters:</b> <code>doctor_id</code> (int)</li>
                    <li><b>Response Body (Success - 200 OK, JSON):</b>
                        <code>{'{"success": true, "schedule": [{"appointment_date": "YYYY-MM-DD", ..., "slot_status": "open/close", "hospital_id": "string"}]}'}</code>
                    </li>
                    <li><b>Response Body (Error, JSON):</b>
                        <code>{'{"success": false, "error": "сообщение об ошибке"}'}</code></li>
                </ul>
            </li>
            <li>
                <b>GET <code>/junior_admin_schedule?junior_admin_id=&lt;id&gt;&amp;doctor_id=&lt;id&gt;</code></b> <i>(Для
                Junior Admin)</i>
                <ul>
                    <li><b>Описание:</b> Получение расписания врача в больнице младшего администратора.</li>
                    <li><b>Request Parameters:</b> <code>junior_admin_id</code> (int), <code>doctor_id</code> (int)</li>
                    <li><b>Response Body (Success - 200 OK, JSON):</b>
                        <code>{'{"success": true, "schedule": [...]}'}</code></li>
                    <li><b>Response Body (Error, JSON):</b>
                        <code>{'{"success": false, "error": "сообщение об ошибке"}'}</code></li>
                    <li><b>Примечание:</b> Текущая реализация на сервере для GET-запроса неверно обрабатывает параметры.
                        Требуется исправление (см. TODO B.3).
                    </li>
                </ul>
            </li>
            <li>
                <b>POST <code>/book_appointment</code></b> <i>(Для Patient)</i>
                <ul>
                    <li><b>Описание:</b> Запись пациента на прием.</li>
                    <li>
                        <b>Request Body (JSON):</b>
                        <code>{'{"doctor_id": int, "hospital_id": int, "appointment_date": "YYYY-MM-DD", "appointment_time": "HH:MM:SS", "patient_id": int}'}</code>
                    </li>
                    <li><b>Response Body (Success - 200 OK, JSON):</b>
                        <code>{'{"success": true, "message": "Booked successfully"}'}</code></li>
                    <li><b>Response Body (Error, JSON):</b>
                        <code>{'{"success": false, "error": "сообщение об ошибке"}'}</code> (статусы 400, 404, 409, 500)
                    </li>
                </ul>
            </li>
            <li>
                <b>PATCH <code>/cancel_appointment</code></b> <i>(Для Patient)</i>
                <ul>
                    <li><b>Описание:</b> Отмена записи на прием.</li>
                    <li>
                        <b>Request Body (JSON):</b> <code>{'{"record_id": int, "patient_id": int}'}</code>
                    </li>
                    <li><b>Response Body (Success - 200 OK, JSON):</b>
                        <code>{'{"success": true, "message": "Appointment cancelled / Cancelled and notified waitlister"}'}</code>
                    </li>
                    <li><b>Response Body (Error, JSON):</b>
                        <code>{'{"success": false, "error": "сообщение об ошибке"}'}</code> (статусы 400, 404, 500)
                    </li>
                </ul>
            </li>
            <li>
                <b>GET <code>/get_patient_appointments?patient_id=&lt;id&gt;</code></b> <i>(Для Patient)</i>
                <ul>
                    <li><b>Описание:</b> Получение списка записей пациента.</li>
                    <li><b>Request Parameters:</b> <code>patient_id</code> (int)</li>
                    <li><b>Response Body (Success - 200 OK, JSON):</b>
                        <code>{'{"success": true, "appointments": [{"appointment_date": "YYYY-MM-DD", ...}]}'}</code>
                    </li>
                    <li><b>Response Body (Error, JSON):</b>
                        <code>{'{"success": false, "error": "сообщение об ошибке"}'}</code></li>
                </ul>
            </li>
            <li>
                <b>GET <code>/get_cancelled_slots?doctor_id=&lt;id&gt;</code></b> <i>(Для Patient)</i>
                <ul>
                    <li><b>Описание:</b> Получение списка отмененных слотов для указанного врача.</li>
                    <li><b>Request Parameters:</b> <code>doctor_id</code> (int)</li>
                    <li><b>Response Body (Success - 200 OK, JSON):</b>
                        <code>{'{"success": true, "slots": [{"record_id": int, "date": "YYYY-MM-DD", "time": "HH:MM:SS"}]}'}</code>
                    </li>
                    <li><b>Response Body (Error, JSON):</b>
                        <code>{'{"success": false, "error": "сообщение об ошибке"}'}</code></li>
                </ul>
            </li>
        </ol>

        <hr/>

        <h3>Поиск Врачей и Фильтры (Для Patient)</h3>
        <ol start={27}>
            <li>
                <b>POST <code>/search_doctors</code></b>
                <ul>
                    <li><b>Описание:</b> Поиск врачей по различным критериям.</li>
                    <li>
                        <b>Request Body (JSON):</b>
                        <code>{'{"region": "string" (opt, "-" if any), "settlement_type": "string" (opt, "-" if any), "settlement_name": "string" (opt, "-" if any), "full_name": "string" (opt, "-" if any, название больницы), "specialty": "string" (opt, "-" if any), "sort_by_rating": boolean (opt, default false)}'}</code>
                    </li>
                    <li><b>Response Body (Success - 200 OK, JSON):</b>
                        <code>{'{"success": true, "doctors": [{"fio": "string", "specialty": "string", ...}]}'}</code>
                    </li>
                    <li><b>Response Body (Error - 500 Internal Server Error, JSON):</b>
                        <code>{'{"success": false, "error": "Database error"}'}</code></li>
                    <li><b>Примечание:</b> Клиентская функция <code>get_doctors_for_patient</code> должна использовать
                        этот эндпоинт для фильтрации.
                    </li>
                </ul>
            </li>
            <li>
                <b>GET <code>/get_regions</code></b>
                <ul>
                    <li><b>Описание:</b> Получение списка уникальных регионов больниц.</li>
                    <li><b>Response Body (Success - 200 OK, JSON):</b>
                        <code>{'{"success": true, "regions": ["-", "Region1", ...]}'}</code></li>
                </ul>
            </li>
            <li>
                <b>GET <code>/get_settlement_types</code></b>
                <ul>
                    <li><b>Описание:</b> Получение списка уникальных типов населенных пунктов.</li>
                    <li><b>Response Body (Success - 200 OK, JSON):</b>
                        <code>{'{"success": true, "settlement_types": ["-", "Type1", ...]}'}</code></li>
                </ul>
            </li>
            <li>
                <b>GET <code>/get_settlement_names</code></b>
                <ul>
                    <li><b>Описание:</b> Получение списка уникальных названий населенных пунктов.</li>
                    <li><b>Response Body (Success - 200 OK, JSON):</b>
                        <code>{'{"success": true, "settlement_names": ["-", "Name1", ...]}'}</code></li>
                </ul>
            </li>
            <li>
                <b>GET <code>/get_hospital_full_names</code></b>
                <ul>
                    <li><b>Описание:</b> Получение списка полных названий больниц.</li>
                    <li><b>Response Body (Success - 200 OK, JSON):</b>
                        <code>{'{"success": true, "hospital_full_names": ["-", "FullName1", ...]}'}</code></li>
                </ul>
            </li>
            <li>
                <b>GET <code>/get_specialties</code></b>
                <ul>
                    <li><b>Описание:</b> Получение списка уникальных специальностей врачей.</li>
                    <li><b>Response Body (Success - 200 OK, JSON):</b>
                        <code>{'{"success": true, "specialties": ["-", "Specialty1", ...]}'}</code></li>
                </ul>
            </li>
            <li>
                <b>GET <code>/get_doctor_hospitals?doctor_id=&lt;id&gt;</code></b>
                <ul>
                    <li><b>Описание:</b> Получение списка больниц, к которым прикреплен врач.</li>
                    <li><b>Request Parameters:</b> <code>doctor_id</code> (int)</li>
                    <li><b>Response Body (Success - 200 OK, JSON):</b>
                        <code>{'{"success": true, "hospitals": [{"hospital_id": int, "full_name": "string", ...}]}'}</code>
                    </li>
                </ul>
            </li>
        </ol>

        <hr/>

        <h3>Отзывы о Врачах</h3>
        <ol start={34}>
            <li>
                <b>POST <code>/post_doctor_feedback</code></b> <i>(Для Patient)</i>
                <ul>
                    <li><b>Описание:</b> Добавление отзыва о враче.</li>
                    <li>
                        <b>Request Body (JSON):</b>
                        <code>{'{"doctor_ref_id": int, "user_id": int, "text": "string", "rate": int (1-5), "address": "string"}'}</code> (name
                        и date генерируются сервером)
                    </li>
                    <li><b>Response Body (Success - 201 Created, JSON):</b>
                        <code>{'{"success": true, "id": "new_rating_id"}'}</code></li>
                    <li><b>Response Body (Error, JSON):</b>
                        <code>{'{"success": false, "error": "сообщение об ошибке"}'}</code> (статусы 400, 404, 500)
                    </li>
                </ul>
            </li>
            <li>
                <b>PATCH <code>/edit_doctor_feedback</code></b> <i>(Для Patient - автора отзыва)</i>
                <ul>
                    <li><b>Описание:</b> Редактирование собственного отзыва.</li>
                    <li>
                        <b>Request Body (JSON):</b>
                        <code>{'{"rating_id": int, "user_id": int (автор), "rate": int, "text": "string" (opt), "name": "string", "date": "YYYY-MM-DD HH:MM:SS", "address": "string"}'}</code>
                    </li>
                    <li><b>Response Body (Success - 200 OK, JSON):</b> <code>{'{"success": true}'}</code></li>
                    <li><b>Response Body (Error, JSON):</b>
                        <code>{'{"success": false, "error": "сообщение об ошибке"}'}</code> (статусы 400, 403, 404, 500)
                    </li>
                </ul>
            </li>
            <li>
                <b>DELETE <code>/delete_doctor_feedback</code></b> <i>(Для Patient - автора, или Junior Admin, если врач
                из его больницы)</i>
                <ul>
                    <li><b>Описание:</b> Удаление отзыва.</li>
                    <li>
                        <b>Request Body (JSON):</b>
                        <code>{'{"rating_id": int, "admin_id": int (user_id того, кто удаляет)}'}</code>
                    </li>
                    <li><b>Response Body (Success - 200 OK, JSON):</b> <code>{'{"success": true}'}</code></li>
                    <li><b>Response Body (Error, JSON):</b>
                        <code>{'{"success": false, "error": "сообщение об ошибке"}'}</code> (статусы 400, 403, 404, 500)
                    </li>
                </ul>
            </li>
            <li>
                <b>GET <code>/get_doctor_feedback_items?doctor_id=&lt;id&gt;</code></b> <i>(Для Patient)</i>
                <ul>
                    <li><b>Описание:</b> Получение всех отзывов о враче.</li>
                    <li><b>Request Parameters:</b> <code>doctor_id</code> (int)</li>
                    <li><b>Response Body (Success - 200 OK, JSON):</b>
                        <code>{'{"success": true, "ratings": [{"id": int, "text": "string", ...}]}'}</code></li>
                </ul>
            </li>
            <li>
                <b>GET <code>/get_doctor_feedback_calculated?doctor_id=&lt;id&gt;</code></b> <i>(Для Patient)</i>
                <ul>
                    <li><b>Описание:</b> Получение среднего рейтинга врача.</li>
                    <li><b>Request Parameters:</b> <code>doctor_id</code> (int)</li>
                    <li><b>Response Body (Success - 200 OK, JSON):</b>
                        <code>{'{"success": true, "average": "float"}'}</code></li>
                </ul>
            </li>
            <li>
                <b>GET <code>/get_doctor_average_ratings</code></b> <i>(Для Patient)</i>
                <ul>
                    <li><b>Описание:</b> Получение средних рейтингов для всех врачей (взвешенный).</li>
                    <li><b>Response Body (Success - 200 OK, JSON):</b>
                        <code>{'{"success": true, "doctor_average_ratings": ["-", "float", ...]}'}</code> (массив, где
                        индекс может соответствовать doctor_id, если так задумано)
                    </li>
                </ul>
            </li>
        </ol>

        <hr/>

        <h3>Лист Ожидания (Waitlist)</h3>
        <ol start={40}>
            <li>
                <b>POST <code>/add_patient_to_waitlist</code></b> <i>(Для Patient)</i>
                <ul>
                    <li><b>Описание:</b> Добавление пациента в лист ожидания к врачу.</li>
                    <li>
                        <b>Request Body (JSON):</b> <code>{'{"doctor_id": int, "patient_id": int}'}</code> (ФИО и
                        телефон берутся из <code>users</code>)
                    </li>
                    <li><b>Response Body (Success - 200 OK, JSON):</b>
                        <code>{'{"success": true, "message": "Patient added to waitlist successfully"}'}</code></li>
                    <li><b>Response Body (Error, JSON):</b>
                        <code>{'{"success": false, "error": "сообщение об ошибке"}'}</code> (статусы 400, 404, 500)
                    </li>
                </ul>
            </li>
            <li>
                <b>POST <code>/cancel_appointment_from_waitlist</code></b> <i>(Для Patient)</i>
                <ul>
                    <li><b>Описание:</b> Отмена своей записи из листа ожидания.</li>
                    <li>
                        <b>Request Body (JSON):</b> <code>{'{"waitlist_id": int, "patient_id": int}'}</code>
                    </li>
                    <li><b>Response Body (Success - 200 OK, JSON):</b>
                        <code>{'{"success": true, "message": "Removed from waitlist"}'}</code></li>
                    <li><b>Response Body (Error, JSON):</b>
                        <code>{'{"success": false, "error": "сообщение об ошибке"}'}</code> (статусы 400, 403, 500)
                    </li>
                    <li><b>Примечание:</b> Клиент вызывает этот эндпоинт через функцию <code>cancel_waitlist</code>.
                    </li>
                </ul>
            </li>
            <li>
                <b>GET <code>/get_waitlist?doctor_id=&lt;id&gt;&amp;junior_admin_id=&lt;id&gt;</code></b> <i>(Для Junior
                Admin)</i>
                <ul>
                    <li><b>Описание:</b> Получение листа ожидания для врача (если врач прикреплен к больнице
                        администратора).
                    </li>
                    <li><b>Request Parameters:</b> <code>doctor_id</code> (int), <code>junior_admin_id</code> (int)</li>
                    <li><b>Response Body (Success - 200 OK, JSON):</b>
                        <code>{'{"success": true, "waitlist": [{"id": int, "patient_id": int, ...}]}'}</code></li>
                    <li><b>Response Body (Error, JSON):</b>
                        <code>{'{"success": false, "error": "сообщение об ошибке"}'}</code></li>
                </ul>
            </li>
            <li>
                <b>GET <code>/get_waitlist_count?doctor_id=&lt;id&gt;</code></b> <i>(Для Patient, Junior Admin)</i>
                <ul>
                    <li><b>Описание:</b> Получение количества пациентов в листе ожидания у врача.</li>
                    <li><b>Request Parameters:</b> <code>doctor_id</code> (int)</li>
                    <li><b>Response Body (Success - 200 OK, JSON):</b> <code>{'{"success": true, "count": int}'}</code>
                    </li>
                </ul>
            </li>
        </ol>

        <hr/>

        <h3>Вспомогательные и Прочее</h3>
        <ol start={44}>
            <li>
                <b>GET <code>/get_user_id?phone=&lt;phone_number&gt;</code></b>
                <ul>
                    <li><b>Описание:</b> Получение ID пользователя по номеру телефона.</li>
                    <li><b>Request Parameters:</b> <code>phone</code> (string)</li>
                    <li><b>Response Body (Success - 200 OK, JSON):</b> <code>{'{"id": int}'}</code></li>
                    <li><b>Response Body (Error - 404 Not Found, JSON):</b> <code>{'{"error": "User not found"}'}</code>
                    </li>
                </ul>
            </li>
            <li>
                <b>GET <code>/get_user_type?phone=&lt;phone_number&gt;</code></b>
                <ul>
                    <li><b>Описание:</b> Получение типа пользователя по номеру телефона.</li>
                    <li><b>Request Parameters:</b> <code>phone</code> (string)</li>
                    <li><b>Response Body (Success - 200 OK, JSON):</b> <code>{'{"user_type": "string"}'}</code></li>
                    <li><b>Response Body (Error - 404 Not Found, JSON):</b> <code>{'{"error": "User not found"}'}</code>
                    </li>
                </ul>
            </li>
            <li>
                <b>GET <code>/check_doctor_admin_hospital?doctor_id=&lt;id&gt;&amp;admin_id=&lt;id&gt;</code></b> <i>(Для
                Junior Admin)</i>
                <ul>
                    <li><b>Описание:</b> Проверка, привязан ли врач к той же больнице, что и младший администратор.</li>
                    <li><b>Request Parameters:</b> <code>doctor_id</code> (int), <code>admin_id</code> (int, user_id
                        младшего админа)
                    </li>
                    <li><b>Response Body (Success - 200 OK, JSON):</b>
                        <code>{'{"is_valid": boolean, "admin_hospital_id": int, "doctor_id": int}'}</code></li>
                </ul>
            </li>
            <li>
                <b>POST <code>/get_doctor_statistics</code></b> <i>(Для Junior Admin)</i>
                <ul>
                    <li><b>Описание:</b> Получение статистики по врачу (количество пациентов за месяц, средний рейтинг,
                        цена приема). Врач должен быть прикреплен к больнице администратора.
                    </li>
                    <li>
                        <b>Request Body (JSON):</b> <code>{'{"junior_admin_id": int, "doctor_phone": "string"}'}</code>
                    </li>
                    <li><b>Response Body (Success - 200 OK, JSON):</b>
                        <code>{'{"success": true, "patients_count": int, "average_rating": "float", "price": "float"}'}</code>
                    </li>
                    <li><b>Response Body (Error, JSON):</b>
                        <code>{'{"success": false, "error": "сообщение об ошибке"}'}</code> (статусы 400, 403, 404, 500)
                    </li>
                </ul>
            </li>
        </ol>

        <hr/>
        <p className="text-sm text-gray-400">Последнее обновление: 2025-06-06 00:58:12</p>
    </main>
}