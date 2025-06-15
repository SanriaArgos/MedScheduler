"use client";

import { useState, useEffect } from "react";
import { useRouter } from "next/navigation";
import Link from "next/link";
import { formatPhoneDisplay, formatPhoneForAPI, validatePhone } from '../../../../utils/phoneFormatter';

export default function JuniorAdminDoctorsPage() {
    const [loading, setLoading] = useState(true);
    const [doctors, setDoctors] = useState([]); // Все врачи
    const [hospitalDoctors, setHospitalDoctors] = useState([]); // Врачи текущей больницы
    const [error, setError] = useState("");
    const [success, setSuccess] = useState("");
    const router = useRouter();

    // Состояние для данных администратора
    const [adminId, setAdminId] = useState(null);
    const [hospitalId, setHospitalId] = useState(null);
    const [hospitalName, setHospitalName] = useState("");    // Состояние для добавления нового врача
    const [isAddingDoctor, setIsAddingDoctor] = useState(false);
    const [newDoctorData, setNewDoctorData] = useState({
        lastName: "",
        firstName: "",
        patronymic: "",
        phone: "",
        specialty: "",
        experience: "",
        education: "",
        price: "",
    });

    // Состояние для прикрепления/открепления врача
    const [isAttaching, setIsAttaching] = useState(false);
    const [selectedDoctorId, setSelectedDoctorId] = useState(null);
    const [selectedDoctorName, setSelectedDoctorName] = useState("");
    const [selectedAction, setSelectedAction] = useState("attach"); // "attach" или "detach"

    // Поиск врачей
    const [searchQuery, setSearchQuery] = useState("");
    const [filteredDoctors, setFilteredDoctors] = useState([]);

    useEffect(() => {
        // Проверка аутентификации
        const isLoggedIn = localStorage.getItem('isLoggedIn');
        const userData = JSON.parse(localStorage.getItem('medSchedulerUser') || '{}');

        if (!isLoggedIn || !userData.userId || userData.userType !== 'junior') {
            router.push('/login');
            return;
        }

        setAdminId(userData.userId);

        // Получаем ID больницы, привязанной к администратору
        const fetchHospitalInfo = async () => {
            try {
                const response = await fetch(`https://api.medscheduler.ru/get_admin_hospital?admin_id=${userData.userId}`);
                const data = await response.json();                if (response.ok && data.success && data.hospital) {
                    setHospitalId(data.hospital.hospital_id);
                    setHospitalName(data.hospital.full_name);

                    // После получения информации о больнице загружаем врачей
                    fetchAllData();
                } else {
                    setError("Не удалось получить информацию о больнице");
                    setLoading(false);
                }
            } catch (err) {
                console.error("Error fetching hospital info:", err);
                setError("Не удалось получить информацию о больнице");
                setLoading(false);
            }
        };

        fetchHospitalInfo();
    }, [router]);
      useEffect(() => {
        // Фильтрация врачей при изменении поискового запроса
        if (searchQuery) {
            const lowercaseQuery = searchQuery.toLowerCase();
            const filtered = doctors.filter(
                doctor =>
                    doctor.last_name.toLowerCase().includes(lowercaseQuery) ||
                    doctor.first_name.toLowerCase().includes(lowercaseQuery) ||
                    (doctor.patronymic && doctor.patronymic.toLowerCase().includes(lowercaseQuery)) ||
                    doctor.specialty.toLowerCase().includes(lowercaseQuery) ||
                    (doctor.education && doctor.education.toLowerCase().includes(lowercaseQuery)) ||
                    doctor.phone.toLowerCase().includes(lowercaseQuery)
            );
            setFilteredDoctors(filtered);
        } else {
            setFilteredDoctors(doctors);
        }    }, [searchQuery, doctors]);    // Функция для проверки, прикреплен ли врач к текущей больнице
    // ВРЕМЕННО ОТКЛЮЧЕНО: API get_hospital_doctors не работает
    const isDoctorAttachedToHospital = (doctorId) => {
        // Возвращаем false, так как мы не можем получить актуальный статус
        // Кнопки будут работать через API attach/detach, которые сами проверят статус
        return false;
    };const fetchDoctors = async () => {
        try {
            // Получаем всех врачей
            const response = await fetch('https://api.medscheduler.ru/get_doctors');
            const data = await response.json();

            if (response.ok && data.success) {
                setDoctors(data.doctors);
                setFilteredDoctors(data.doctors);
            } else {
                setError(data.error || "Ошибка при получении списка врачей");
            }
        } catch (err) {
            console.error("Error fetching doctors:", err);
            setError("Не удалось подключиться к серверу");
        }
    };    const fetchHospitalDoctors = async () => {
        if (!hospitalId) return;
        
        // ВРЕМЕННОЕ РЕШЕНИЕ: API get_hospital_doctors сломан
        // Используем пустой массив и полагаемся на проверку через attach/detach API
        console.log("Skipping get_hospital_doctors due to server error");
        setHospitalDoctors([]);
        
        // TODO: Исправить get_hospital_doctors на сервере или изменить get_doctors
        // чтобы он возвращал hospital_ids для каждого врача
        
        /*
        try {
            const response = await fetch(`https://api.medscheduler.ru/get_hospital_doctors?hospital_id=${hospitalId}`);
            const data = await response.json();

            if (response.ok && data.success) {
                setHospitalDoctors(data.doctors || []);
            } else {
                console.error("Error fetching hospital doctors:", data.error);
                setHospitalDoctors([]);
            }
        } catch (err) {
            console.error("Error fetching hospital doctors:", err);
            setHospitalDoctors([]);
        }
        */
    };

    const fetchAllData = async () => {
        setLoading(true);
        await Promise.all([fetchDoctors(), fetchHospitalDoctors()]);
        setLoading(false);
    };

    const handleNewDoctorChange = (e) => {
        const { name, value } = e.target;
        setNewDoctorData((prev) => ({ ...prev, [name]: value }));
    };

    const handleNewDoctorPhoneChange = (e) => {
        setNewDoctorData((prev) => ({ ...prev, phone: e.target.value }));
    };

    const handleAddDoctor = async (e) => {
        e.preventDefault();
        setLoading(true);
        setError("");
        setSuccess("");

        if (!validatePhone(newDoctorData.phone)) {
            setError("Введите корректный  номер телефона для нового врача.");
            setLoading(false);
            return;
        }        if (!hospitalId) {
            setError("Не удалось определить больницу для добавления врача. Пожалуйста, обновите страницу.");
            setLoading(false);
            return;
        }        const payload = {
            last_name: newDoctorData.lastName,
            first_name: newDoctorData.firstName,
            patronymic: newDoctorData.patronymic,
            phone: formatPhoneForAPI(newDoctorData.phone),
            specialty: newDoctorData.specialty,
            experience: parseInt(newDoctorData.experience),
            education: newDoctorData.education,
            price: parseInt(newDoctorData.price),
        };

        try {
            const response = await fetch("https://api.medscheduler.ru/add_doctor", {
                method: "POST",
                headers: { "Content-Type": "application/json" },
                body: JSON.stringify(payload),
            });

            const data = await response.json();

            if (response.ok && data.success) {
                setSuccess("Врач успешно добавлен");                // Сбрасываем форму
                setNewDoctorData({
                    lastName: "",
                    firstName: "",
                    patronymic: "",
                    phone: "",
                    specialty: "",
                    experience: "",
                    education: "",
                    price: "",
                });                // Обновляем список врачей
                fetchAllData();

                // Скрываем форму через 2 секунды
                setTimeout(() => {
                    setIsAddingDoctor(false);
                    setSuccess("");
                }, 10000);
            } else {
                setError(data.error || "Ошибка при добавлении врача");
            }
        } catch (err) {
            console.error("Error adding doctor:", err);
            setError("Не удалось подключиться к серверу");
        } finally {
            setLoading(false);
        }
    };

    const handleAttachDoctor = async () => {
        if (!selectedDoctorId || !hospitalId || !adminId) {
            setError("Не удалось выполнить операцию. Отсутствуют необходимые данные.");
            return;
        }

        setError("");
        setSuccess("");

        try {
            const endpoint = selectedAction === "attach"
                ? 'https://api.medscheduler.ru/attach_doctor_to_hospital'
                : 'https://api.medscheduler.ru/detach_doctor_from_hospital';

            const response = await fetch(endpoint, {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({
                    doctor_id: parseInt(selectedDoctorId),
                    hospital_id: parseInt(hospitalId),
                    junior_admin_id: parseInt(adminId)
                }),
            });

            const data = await response.json();            if (response.ok && data.success) {                setSuccess(selectedAction === "attach"
                    ? `Врач ${selectedDoctorName} добавлен в больницу`
                    : `Врач ${selectedDoctorName} убран из больницы`
                );

                // Обновляем данные
                fetchAllData();

                // Сбрасываем форму
                setSelectedDoctorId(null);
                setSelectedDoctorName("");

                // Скрываем форму через 2 секунды
                setTimeout(() => {
                    setIsAttaching(false);
                    setSuccess("");
                }, 10000);            } else {
                // Более подробная обработка ошибок
                let errorMessage = data.error || `Ошибка при ${selectedAction === "attach" ? "добавлении" : "удалении"} врача`;
                
                if (data.error && data.error.includes("already attached")) {
                    errorMessage = "Врач уже работает в этой больнице";
                } else if (data.error && data.error.includes("not found")) {
                    errorMessage = "Врач не найден в системе";
                } else if (selectedAction === "detach" && data.error) {
                    errorMessage = "Врач не работает в этой больнице";
                }
                
                setError(errorMessage);
            }
        } catch (err) {
            console.error(`Error ${selectedAction} doctor:`, err);
            setError("Не удалось подключиться к серверу");
        }
    };

    const startAttach = (doctor, action) => {
        setSelectedDoctorId(doctor.doctor_id);
        setSelectedDoctorName(`${doctor.last_name} ${doctor.first_name} ${doctor.patronymic || ""}`);
        setSelectedAction(action);
        setIsAttaching(true);
        setError("");
        setSuccess("");
    };

    if (loading) {
        return (
            <div className="min-h-[calc(80vh-theme(space.36))] flex items-center justify-center">
                <div className="text-xl text-gray-600">Загрузка данных...</div>
            </div>
        );
    }

    return (
        <div className="max-w-6xl mx-auto py-8 px-4">
            <h1 className="text-3xl font-bold text-main2 mb-8">Управление врачами</h1>

            {error && (
                <div className="bg-red-100 border border-red-400 text-red-700 px-4 py-3 rounded mb-4">
                    {error}
                </div>
            )}

            {success && (
                <div className="bg-green-100 border border-green-400 text-green-700 px-4 py-3 rounded mb-4">
                    {success}
                </div>
            )}

            <div className="bg-white shadow-md rounded-lg p-6 mb-6">
                <div className="flex flex-col md:flex-row justify-between md:items-center mb-6">
                    <div>
                        <h2 className="text-xl font-semibold text-main2">Ваша больница</h2>
                        <p className="text-gray-600 mt-1">{hospitalName || "Больница не привязана"}</p>
                    </div>
                    <div className="mt-4 md:mt-0">
                        <button
                            onClick={() => setIsAddingDoctor(true)}
                            className="px-4 py-2 bg-main text-white rounded hover:bg-main2 transition-colors"
                        >
                            Добавить нового врача
                        </button>
                    </div>
                </div>

                {/* Поиск врачей */}
                <div className="mb-6">
                    <label htmlFor="search" className="block text-sm font-medium text-gray-700 mb-1">
                        Поиск врачей
                    </label>                    <input
                        type="text"
                        id="search"
                        value={searchQuery}
                        onChange={(e) => setSearchQuery(e.target.value)}
                        placeholder="Поиск по ФИО, специальности, образованию или телефону"
                        className="w-full px-3 py-2 border border-gray-300 rounded-md focus:outline-none focus:ring-1 focus:ring-main"
                    />
                </div>

                {isAddingDoctor && (
                    <div className="mb-6 p-4 bg-gray-50 rounded-lg">
                        <h3 className="text-lg font-medium text-gray-800 mb-4">Добавить нового врача</h3>
                        <form onSubmit={handleAddDoctor}>
                            <div className="grid grid-cols-1 md:grid-cols-2 gap-4 mb-4">
                                <div>
                                    <label htmlFor="lastName" className="block text-sm font-medium text-gray-700 mb-1">
                                        Фамилия *
                                    </label>
                                    <input
                                        type="text"
                                        id="lastName"
                                        name="lastName"
                                        value={newDoctorData.lastName}
                                        onChange={handleNewDoctorChange}
                                        className="w-full px-3 py-2 border border-gray-300 rounded-md focus:outline-none focus:ring-1 focus:ring-main"
                                        required
                                    />
                                </div>
                                <div>
                                    <label htmlFor="firstName" className="block text-sm font-medium text-gray-700 mb-1">
                                        Имя *
                                    </label>
                                    <input
                                        type="text"
                                        id="firstName"
                                        name="firstName"
                                        value={newDoctorData.firstName}
                                        onChange={handleNewDoctorChange}
                                        className="w-full px-3 py-2 border border-gray-300 rounded-md focus:outline-none focus:ring-1 focus:ring-main"
                                        required
                                    />
                                </div>
                                <div>
                                    <label htmlFor="patronymic" className="block text-sm font-medium text-gray-700 mb-1">
                                        Отчество
                                    </label>
                                    <input
                                        type="text"
                                        id="patronymic"
                                        name="patronymic"
                                        value={newDoctorData.patronymic}
                                        onChange={handleNewDoctorChange}
                                        className="w-full px-3 py-2 border border-gray-300 rounded-md focus:outline-none focus:ring-1 focus:ring-main"
                                    />
                                </div>
                                <div>
                                    <label htmlFor="phone" className="block text-sm font-medium text-gray-700 mb-1">
                                        Телефон * (11 цифр)
                                    </label>
                                    <input
                                        type="tel"
                                        name="phone"
                                        value={formatPhoneDisplay(newDoctorData.phone)}
                                        onChange={handleNewDoctorPhoneChange}
                                        className="w-full px-3 py-2 border border-gray-300 rounded-md"
                                        placeholder="+7 (999) 123-45-67"
                                        required
                                    />
                                </div>
                                <div>
                                    <label htmlFor="specialty" className="block text-sm font-medium text-gray-700 mb-1">
                                        Специальность *
                                    </label>
                                    <input
                                        type="text"
                                        id="specialty"
                                        name="specialty"
                                        value={newDoctorData.specialty}
                                        onChange={handleNewDoctorChange}
                                        className="w-full px-3 py-2 border border-gray-300 rounded-md focus:outline-none focus:ring-1 focus:ring-main"
                                        required
                                    />
                                </div>
                                <div>
                                    <label htmlFor="experience" className="block text-sm font-medium text-gray-700 mb-1">
                                        Опыт работы (лет) *
                                    </label>
                                    <input
                                        type="number"
                                        id="experience"
                                        name="experience"
                                        value={newDoctorData.experience}
                                        onChange={handleNewDoctorChange}
                                        min="0"
                                        className="w-full px-3 py-2 border border-gray-300 rounded-md focus:outline-none focus:ring-1 focus:ring-main"
                                        required
                                    />                                </div>
                                <div>
                                    <label htmlFor="education" className="block text-sm font-medium text-gray-700 mb-1">
                                        Образование *
                                    </label>
                                    <input
                                        type="text"
                                        id="education"
                                        name="education"
                                        value={newDoctorData.education}
                                        onChange={handleNewDoctorChange}
                                        className="w-full px-3 py-2 border border-gray-300 rounded-md focus:outline-none focus:ring-1 focus:ring-main"
                                        placeholder="Название учебного заведения"
                                        required
                                    />
                                </div>
                                <div>
                                    <label htmlFor="price" className="block text-sm font-medium text-gray-700 mb-1">
                                        Стоимость приёма (руб.) *
                                    </label>
                                    <input
                                        type="number"
                                        id="price"
                                        name="price"
                                        value={newDoctorData.price}
                                        onChange={handleNewDoctorChange}
                                        min="0"
                                        className="w-full px-3 py-2 border border-gray-300 rounded-md focus:outline-none focus:ring-1 focus:ring-main"
                                        placeholder="1500"
                                        required
                                    />
                                </div>
                            </div>

                            <div className="flex gap-3">
                                <button
                                    type="submit"
                                    className="px-4 py-2 bg-main text-white rounded hover:bg-main2 transition-colors"
                                >
                                    Добавить врача
                                </button>
                                <button
                                    type="button"
                                    onClick={() => setIsAddingDoctor(false)}
                                    className="px-4 py-2 bg-gray-200 text-gray-800 rounded hover:bg-gray-300 transition-colors"
                                >
                                    Отмена
                                </button>
                            </div>
                        </form>
                    </div>
                )}

                {isAttaching && (
                    <div className="mb-6 p-4 bg-gray-50 rounded-lg">                        <h3 className="text-lg font-medium text-gray-800 mb-4">
                            {selectedAction === "attach" ? "Добавить врача в больницу" : "Убрать врача из больницы"}
                        </h3>
                        <div className="mb-4">
                            <p className="text-sm text-gray-700 mb-2">Врач:</p>
                            <p className="font-medium">{selectedDoctorName}</p>
                        </div>
                        <div className="mb-4">
                            <p className="text-sm text-gray-700 mb-2">Больница:</p>
                            <p className="font-medium">{hospitalName}</p>
                        </div>
                        <div className="flex gap-3">                            <button
                                onClick={handleAttachDoctor}
                                className="px-4 py-2 bg-main text-white rounded hover:bg-main2 transition-colors"
                            >
                                {selectedAction === "attach" ? "Добавить" : "Убрать"}
                            </button>
                            <button
                                onClick={() => setIsAttaching(false)}
                                className="px-4 py-2 bg-gray-200 text-gray-800 rounded hover:bg-gray-300 transition-colors"
                            >
                                Отмена
                            </button>
                        </div>
                    </div>
                )}                {/* Список врачей */}
                <div>                    <div className="flex justify-between items-center mb-4">
                        <h3 className="text-lg font-medium text-gray-800">Список врачей</h3>
                        <div className="text-sm text-gray-600 bg-orange-50 px-3 py-2 rounded border border-orange-200">
                            ⚠️ Статус врачей временно недоступен (проблема с API). Кнопки работают.
                        </div>
                    </div>

                    {filteredDoctors.length === 0 ? (
                        <p className="text-gray-500 text-center py-4">Врачи не найдены</p>
                    ) : (
                        <div className="overflow-x-auto">
                            <table className="min-w-full divide-y divide-gray-200">
                                <thead className="bg-gray-50">
                                    <tr>
                                        <th scope="col" className="px-6 py-3 text-left text-xs font-medium text-gray-500 uppercase tracking-wider">
                                            ФИО
                                        </th>
                                        <th scope="col" className="px-6 py-3 text-left text-xs font-medium text-gray-500 uppercase tracking-wider">
                                            Специальность
                                        </th>
                                        <th scope="col" className="px-6 py-3 text-left text-xs font-medium text-gray-500 uppercase tracking-wider">
                                            Опыт
                                        </th>
                                        <th scope="col" className="px-6 py-3 text-left text-xs font-medium text-gray-500 uppercase tracking-wider">
                                            Образование
                                        </th>
                                        <th scope="col" className="px-6 py-3 text-left text-xs font-medium text-gray-500 uppercase tracking-wider">
                                            Цена
                                        </th>                                        <th scope="col" className="px-6 py-3 text-left text-xs font-medium text-gray-500 uppercase tracking-wider">
                                            Телефон
                                        </th>
                                        <th scope="col" className="px-6 py-3 text-left text-xs font-medium text-gray-500 uppercase tracking-wider">
                                            Статус
                                        </th>
                                        <th scope="col" className="px-6 py-3 text-left text-xs font-medium text-gray-500 uppercase tracking-wider">
                                            Действия
                                        </th>
                                    </tr>
                                </thead>
                                <tbody className="bg-white divide-y divide-gray-200">
                                    {filteredDoctors.map((doctor) => (
                                        <tr key={doctor.doctor_id}>
                                            <td className="px-6 py-4 whitespace-nowrap">
                                                <div className="text-sm font-medium text-gray-900">
                                                    {doctor.last_name} {doctor.first_name} {doctor.patronymic || ""}
                                                </div>
                                            </td>
                                            <td className="px-6 py-4 whitespace-nowrap">
                                                <div className="text-sm text-gray-500">{doctor.specialty}</div>
                                            </td>
                                            <td className="px-6 py-4 whitespace-nowrap">
                                                <div className="text-sm text-gray-500">
                                                    {doctor.experience} {doctor.experience === 1 ? "год" : doctor.experience < 5 ? "года" : "лет"}
                                                </div>
                                            </td>
                                            <td className="px-6 py-4 whitespace-nowrap">
                                                <div className="text-sm text-gray-500">{doctor.education || "Не указано"}</div>
                                            </td>
                                            <td className="px-6 py-4 whitespace-nowrap">
                                                <div className="text-sm text-gray-500">
                                                    {doctor.price ? `${doctor.price} ₽` : "Не указано"}
                                                </div>
                                            </td>                                            <td className="px-6 py-4 whitespace-nowrap">
                                                <div className="text-sm text-gray-500">{doctor.phone}</div>
                                            </td>                                            <td className="px-6 py-4 whitespace-nowrap">
                                                <span className="inline-flex items-center px-2.5 py-0.5 rounded-full text-xs font-medium bg-yellow-100 text-yellow-800">
                                                    Статус неизвестен
                                                </span>
                                            </td><td className="px-6 py-4 whitespace-nowrap text-sm font-medium">                                                <div className="flex flex-col space-y-1">
                                                    <button
                                                        onClick={() => startAttach(doctor, "attach")}
                                                        className="text-main hover:text-main2 text-left text-sm"
                                                        title="Попробовать добавить врача в нашу больницу"
                                                    >
                                                        ➕ Добавить
                                                    </button>
                                                    <button
                                                        onClick={() => startAttach(doctor, "detach")}
                                                        className="text-red-600 hover:text-red-800 text-left text-sm"
                                                        title="Попробовать убрать врача из нашей больницы"
                                                    >
                                                        ➖ Убрать
                                                    </button>
                                                </div>
                                            </td>
                                        </tr>
                                    ))}
                                </tbody>
                            </table>
                        </div>
                    )}
                </div>
            </div>

            <div className="flex justify-between">
                <Link href="/junior-admin/profile" className="text-main hover:text-main2 transition-colors">
                    ← Профиль
                </Link>
                <Link href="/junior-admin/schedule" className="text-main hover:text-main2 transition-colors">
                    Управление расписанием →
                </Link>
            </div>
        </div>
    );
}
